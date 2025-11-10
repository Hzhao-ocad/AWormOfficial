#include <Arduino.h>
#include <stdint.h>

// Writes an increasing random number to Serial until it reaches 50,000,000,000.
// The increments slowly grow as the value grows so the numbers trend upward.

static uint64_t value = 0ULL;
static const uint64_t MAX_VALUE = 50000000000ULL; // 50 billion

// Convert uint64_t to a null-terminated decimal string (no libc %llu reliance).
// Returns an Arduino String for easy Serial.print usage.
String u64ToString(uint64_t v) {
	if (v == 0) return String("0");
	char buf[32];
	buf[31] = '\0';
	int pos = 30;
	while (v > 0 && pos >= 0) {
		uint8_t digit = (uint8_t)(v % 10ULL);
		buf[pos--] = '0' + digit;
		v /= 10ULL;
	}
	return String(&buf[pos + 1]);
}

// Return a human-readable label like "12.3 thousand", "4 million", "2.5 billion".
String humanReadable(uint64_t v) {
	if (v < 1000ULL) return String(u64ToString(v));
	const char* suffix[] = {"thousand", "million", "billion", "trillion"};
	double scaled = (double)v;
	int idx = 0;
	// Scale down by thousands until we land in the correct suffix bucket
	while (scaled >= 1000.0 && idx < 4) {
		scaled /= 1000.0;
		idx++;
	}
	if (idx == 0) return String(u64ToString(v));
	int sufIndex = idx - 1;
	int decimals = (scaled < 10.0) ? 1 : 0;
	// Use Arduino String formatting for floats: String(value, decimals)
	String out = String((float)scaled, decimals);
	out += " ";
	out += suffix[sufIndex];
	return out;
}

// Return a string of exclamation marks proportional to the value (0..20)
String exclamationMarks(uint64_t v) {
	uint32_t count = 0;
	if (v > 0) {
		count = (uint32_t)((v * 20ULL) / MAX_VALUE);
		if (count > 20U) count = 20U;
	}
	String s = "";
	for (uint32_t i = 0; i < count; ++i) s += "!";
	return s;
}

void setup() {
	Serial.begin(115200);
	// Give the serial some time
	delay(50);
	// Try to seed the RNG with analog noise if available.
	// If analogRead is not available or floating, we also mix in micros().
#if defined(ANALOG_INPUT)
	randomSeed(analogRead(A0) ^ (uint32_t)micros());
#else
	randomSeed((uint32_t)micros());
#endif
	Serial.println("Starting increasing-random generator (max 50,000,000,000)");
}

void loop() {
	if (value >= MAX_VALUE) {
		Serial.println("Reached max value. Stopping updates.");
		while (true) {
			delay(1000);
		}
	}

	// Base random increment from 1..999,999 (larger so 2s holds become possible)
	uint64_t base = (uint64_t)random(1, 10000000000);
	// Growth term so increments increase as value grows (value / 1,000,000)
	uint64_t growth = value / 1000000ULL;
	uint64_t increment = base + growth;

	// Prevent overshoot beyond MAX_VALUE
	if (value + increment > MAX_VALUE) {
		increment = MAX_VALUE - value;
	}

		value += increment;

			// Print the number as decimal (portable) plus human-readable label and excitement
			String label = humanReadable(value);
			String bangs = exclamationMarks(value);
			Serial.println(u64ToString(value) + " (" + label + ") " + bangs);

		// Compute a delay based on the delta (increment) value.
		// Larger increments produce a longer wait to build suspense. The delay
		// is clamped to a minimum and a maximum (max 2000 ms as requested).
	// We estimate a reasonable maximum expected increment based on the
	// current growth formula: growth ~= MAX_VALUE / 1,000,000, plus base up to ~1,000,000.
	const uint64_t expected_max_increment = (MAX_VALUE / 10000000000ULL) + 10000000000ULL;
		uint32_t wait_ms = (uint32_t)((increment * 2000ULL) / expected_max_increment);
		if (wait_ms > 2000U) wait_ms = 2000U;
		if (wait_ms < 20U) wait_ms = 20U; // avoid zero/too-fast prints
		delay(wait_ms);
}
