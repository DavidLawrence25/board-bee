#ifndef BOARD_BEE_LIBS_ANSI_H_
#define BOARD_BEE_LIBS_ANSI_H_

#include "aliases.h"

#include <array>
#include <ostream>
#include <sstream>

namespace rose::ansi {

// Represents a simple 24-bit RGB color.
struct Color {
  u8 red, green, blue;
};

// Original 8 colors supported in all terminals with ANSI escape code support.
enum class BasicPalette { kBlack, kRed, kGreen, kYellow,
                          kBlue, kMagenta, kCyan, kWhite };

// Converts coordinates in the 6x6x6 color cube to a unique color code.
inline u8 ColorCube6(const u8 r, const u8 g, const u8 b) {
  if (r > 5 || g > 5 || b > 5) {
    std::stringstream error_msg;
    // We have to cast to u16's so it doesn't print them as chars.
    error_msg << '(' << static_cast<u16>(r) << ", " << static_cast<u16>(g)
              << ", " << static_cast<u16>(b)
              << ") is not a valid coordinate on the 6x6x6 color cube";
    throw std::runtime_error(error_msg.str());
  }
  return 16 + 36 * r + 6 * g + b;
}

// Represents an ANSI control sequence.
// Useful for manipulating the terminal output and cursor position.
class CtrlSequence {
 public:
  // The maximum number of arguments in a single control sequence.
  static constexpr size_t kMaxArgs = 5;
  static_assert(kMaxArgs != 0);

  // Characters used to specify which "function" to call.
  enum class FunctionCode {
    kCursorUp = 'A',
    kCursorDown = 'B',
    kCursorForward = 'C',
    kCursorBack = 'D',
    kCursorNextLine = 'E',
    kCursorPreviousLine = 'F',
    kCursorHorizontalAbsolute = 'G',
    kCursorPosition = 'H',
    kCursorShow = 'h',
    kCursorHide = 'l',
    kEraseInDisplay = 'J',
    kEraseInLine = 'K',
    kScrollUp = 'S',
    kScrollDown = 'T',
    kSaveCursorPosition = 's',
    kRestoreCursorPosition = 'u',
    kSetGraphicsRendition = 'm'
  };

  // Moves the cursor up by `n` cells.
  // Stops at the top edge of the screen.
  static CtrlSequence CursorUp(u16 n = 1);
  // Moves the cursor down by `n` cells.
  // Stops at the bottom edge of the screen.
  static CtrlSequence CursorDown(u16 n = 1);
  // Moves the cursor to the right by `n` cells.
  // Stops at the right edge of the screen.
  static CtrlSequence CursorForward(u16 n = 1);
  // Moves the cursor to the left by `n` cells.
  // Stops at the left edge of the screen.
  static CtrlSequence CursorBack(u16 n = 1);
  // Moves the cursor to the start of the line `n` lines down.
  static CtrlSequence CursorNextLine(u16 n = 1);
  // Moves the cursor to the start of the line `n` lines up.
  static CtrlSequence CursorPreviousLine(u16 n = 1);
  // Sets the cursor's column position to `column`.
  static CtrlSequence CursorHorizontalAbsolute(u16 column = 1);
  // Sets the cursor's position to `row`:`column`.
  static CtrlSequence CursorPosition(u16 column = 1, u16 row = 1);
  // Sets the cursor to be visible.
  static CtrlSequence CursorShow();
  // Sets the cursor to be invisible.
  static CtrlSequence CursorHide();
  // Clears the screen from the cursor's position to the end.
  static CtrlSequence ClearCursorToScreenEnd();
  // Clears the screen from the start to the cursor's position.
  static CtrlSequence ClearScreenStartToCursor();
  // Clears the whole screen.
  // Also moves the cursor to the top-left corner of the screen on DOS ANSI.SYS.
  static CtrlSequence ClearScreen();
  // Clears* the whole screen and contents of the scrollback buffer.
  // *None of the terminals I've tried actually clear the screen for this.
  static CtrlSequence ClearScreenAndBuffer();
  // Clears the screen from the cursor's position to the end of the line.
  static CtrlSequence ClearCursorToLineEnd();
  // Clears the screen from the start of the line to the cursor's position.
  static CtrlSequence ClearLineStartToCursor();
  // Clears the screen for this line only.
  static CtrlSequence ClearLine();
  // Scrolls up `n` lines and inserts `n` new lines at the bottom of the screen.
  static CtrlSequence ScrollUp(u16 n = 1);
  // Scrolls donw `n` lines and inserts `n` new lines at the top of the screen.
  static CtrlSequence ScrollDown(u16 n = 1);
  // Saves the current cursor position.
  static CtrlSequence SaveCursorPosition();
  // Restores the cursor position to its last saved position.
  static CtrlSequence RestoreCursorPosition();
  // Reset all graphical effects.
  static CtrlSequence SgrReset();
  // Makes text bold.
  static CtrlSequence SgrBold();
  // Italicizes text.
  static CtrlSequence SgrItalic();
  // Underlines text.
  static CtrlSequence SgrUnderline();
  // Sets the text color to a basic `color`.
  // If `bright` is true, uses the brighter variant of `color`.
  static CtrlSequence SgrTextColor(BasicPalette color, bool bright = false);
  // Sets the text color via an 8-bit `color_code`.
  // 0x00 - 0x07 => BasicPalette
  // 0x08 - 0x0F => BasicPalette (brighter variant)
  // 0x10 - 0xE7 => 6x6x6 Color Cube
  // 0xE8 - 0xFF => Grayscale (dark to light)
  static CtrlSequence SgrTextColor(u8 color_code);
  // Sets the text color to an RGB `color`.
  static CtrlSequence SgrTextColor(Color color);
  // Sets the background color to a basic `color`.
  // If `bright is true, uses the brighter variant of `color`.
  static CtrlSequence SgrBgColor(BasicPalette color, bool bright = false);
  // Sets the background color via an 8-bit `color_code`.
  // 0x00 - 0x07 => BasicPalette
  // 0x08 - 0x0F => BasicPalette (brighter variant)
  // 0x10 - 0xE7 => 6x6x6 Color Cube
  // 0xE8 - 0xFF => Grayscale (dark to light)
  static CtrlSequence SgrBgColor(u8 color_code);
  // Sets the background color to an RGB `color`.
  static CtrlSequence SgrBgColor(Color color);

  CtrlSequence(const FunctionCode code,
               const std::array<opt<u16>, kMaxArgs> &args)
      : function_code_(code), args_(args) {}
  CtrlSequence(const CtrlSequence &other) = default;
  CtrlSequence &operator=(const CtrlSequence &other) = default;
  CtrlSequence(CtrlSequence &&other) = default;
  CtrlSequence &operator=(CtrlSequence &&other) = default;

  // Converts this sequence to a functional ANSI control sequence.
  explicit operator str() const;
  friend std::ostream &operator<<(std::ostream &out, const CtrlSequence &seq);

 private:
  FunctionCode function_code_;
  std::array<opt<u16>, kMaxArgs> args_;
};

}  // namespace rose::ansi

#endif  // BOARD_BEE_LIBS_ANSI_H_
