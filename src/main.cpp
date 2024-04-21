#include "BluetoothA2DPSource.h"

static constexpr char const *speaker_name = "JBL Go 3";

static BluetoothA2DPSource a2dp_source;

static int32_t callback_write_data_frames(Frame *frame, int32_t frame_count);

void setup() {
  a2dp_source.set_auto_reconnect(false);
  a2dp_source.start(speaker_name, callback_write_data_frames);
  a2dp_source.set_volume(50);
}

void loop() {
  // to prevent watchdog in release > 1.0.6
  delay(1000);
}

// The supported audio codec in ESP32 A2DP is SBC. SBC audio stream is encoded
// from PCM data normally formatted as 44.1kHz sampling rate, two-channel 16-bit
// sample data
static int32_t callback_write_data_frames(Frame *frame, int32_t const frame_count) {
  static int16_t amp = 0;

  // ESP_LOGD("MAIN", "frame_count: %d", frame_count);

  for (int sample = 0; sample < frame_count; ++sample) {
    frame[sample].channel1 = amp;
    frame[sample].channel2 = frame[sample].channel1;
    amp += 100;
  }

  // to prevent watchdog
  esp_task_wdt_reset();

  return frame_count;
}
