// ==== Driver chọn cho màn hình ====
#define ST7789_DRIVER          // Driver ST7789

// ==== Kích thước màn hình ====
#define TFT_WIDTH  240        // Chiều ngang
#define TFT_HEIGHT 240        // Chiều dọc

// ==== SPI chân kết nối ====
#define TFT_MISO  -1          // Không dùng MISO
#define TFT_MOSI  23          // MOSI (chân dữ liệu)
#define TFT_SCLK  18          // SCLK (SPI Clock)
#define TFT_CS    -1          // Không dùng CS, hoặc gán thử 5 nếu cần
#define TFT_DC     2          // Chân Data/Command
#define TFT_RST    4          // Chân Reset
#define TFT_BL    15          // Chân điều khiển đèn nền (Backlight)

// ==== Thứ tự màu RGB/BGR ====
#define TFT_RGB_ORDER TFT_RGB // Mặc định là RGB. Đảo lại nếu màu bị sai.

// ==== Tốc độ SPI ====
#define SPI_FREQUENCY  40000000  // 40 MHz (giảm xuống nếu bị lỗi hiển thị)

// ==== Căn chỉnh màn hình (Offset) ====
#define TFT_X_OFFSET 0
#define TFT_Y_OFFSET 0         // Thử 80 nếu hiển thị bị lệch

