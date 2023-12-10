/*
 *  libcaca++   C++ bindings for libcaca
 *  Copyright © 2006—2007 Jean-Yves Lamoureux <jylam@lnxscene.org>
 *              2009—2018 Sam Hocevar <sam@hocevar.net>
 *              All Rights Reserved
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What the Fuck You Want
 *  to Public License, Version 2, as published by Sam Hocevar. See
 *  http://www.wtfpl.net/ for more details.
 */

/** \file caca++.h
 *  \author Jean-Yves Lamoureux <jylam@lnxscene.org>
 *  \brief The \e libcaca++ public header.
 *
 *  This header contains the public types and functions that applications
 *  using \e libcaca++ may use.
 */
#ifndef _CACA_PP_H
#define _CACA_PP_H

#include <caca.h>

#undef __class
#if defined(_WIN32) && defined(__LIBCACA_PP__)
#   define __class class __declspec(dllexport)
#elif defined CACA_ENABLE_VISIBILITY
#   define __class class __attribute__((visibility("default")))
#else
#   define __class class
#endif

class Canvas;

__class Charset
{
 public:
    /** \brief Convert a UTF-8 character to UTF-32.
     *
     *  Convert a UTF-8 character read from a string and return its value in
     *  the UTF-32 character set. If the second argument is not null, the total
     *  number of read bytes is written in it.
     *
     *  If a null byte was reached before the expected end of the UTF-8 sequence,
     *  this function returns zero and the number of read bytes is set to zero.
     *
     *  This function never fails, but its behaviour with illegal UTF-8 sequences
     *  is undefined.
     *
     *  \param s A string containing the UTF-8 character.
     *  \param read A pointer to a size_t to store the number of bytes in the
     *         character, or NULL.
     *  \return The corresponding UTF-32 character, or zero if the character
     *  is incomplete.
     */
    uint32_t utf8ToUtf32(char const *s, size_t *read);
    /** \brief Convert a UTF-32 character to UTF-8.
     *
     *  Convert a UTF-32 character read from a string and write its value in
     *  the UTF-8 character set into the given buffer.
     *
     *  This function never fails, but its behaviour with illegal UTF-32 characters
     *  is undefined.
     *
     *  \param buf A pointer to a character buffer where the UTF-8 sequence will
     *  be written.
     *  \param ch The UTF-32 character.
     *  \return The number of bytes written.
     */
    size_t utf32ToUtf8(char *buf, uint32_t ch);
    /** \brief Convert a UTF-32 character to CP437.
     *
     *  Convert a UTF-32 character read from a string and return its value in
     *  the CP437 character set, or "?" if the character has no equivalent.
     *
     *  \param ch The UTF-32 character.
     *  \return The corresponding CP437 character, or "?" if not representable.
     */
    uint8_t utf32ToCp437(uint32_t ch);
    /** \brief Convert a CP437 character to UTF-32.
     *
     *  Convert a CP437 character read from a string and return its value in
     *  the UTF-32 character set, or zero if the character is a CP437 control
     *  character.
     *
     *  \param ch The CP437 character.
     *  \return The corresponding UTF-32 character, or zero if not representable.
     */
    uint32_t cp437ToUtf32(uint8_t ch);
};

/* Ugly, I know */
__class Font
{
 public:
    ~Font();
    /** \brief Load a font from memory for future use.
     *
     *  This function loads a font and returns a handle to its internal
     *  structure. The handle can then be used with caca_render_canvas()
     *  for bitmap output.
     *
     *  Internal fonts can also be loaded: if \c size is set to 0, \c data must
     *  be a string containing the internal font name.
     *
     *  If \c size is non-zero, the \c size bytes of memory at address \c data
     *  are loaded as a font. This memory are must not be freed by the calling
     *  program until the font handle has been freed with caca_free_font().
     *
     *  \throws -1 if an error occurs.
     *
     *  \param data The memory area containing the font or its name.
     *  \param size The size of the memory area, or 0 if the font name is given.
     *  \return A font handle or NULL in case of error.
     */
    Font(void const *data, unsigned int size);
    /** \brief Get available builtin fonts
     *
     *  Return a list of available builtin fonts. The list is a NULL-terminated
     *  array of strings.
     *
     *  This function never fails.
     *
     *  \return An array of strings.
     */
    char const *const * getList(void);
    /** \brief Get a font's standard glyph width.
     *
     *  Return the standard value for the current font's glyphs. Most glyphs in
     *  the font will have this width, except fullwidth characters.
     *
     *  This function never fails.
     *
     *  \return The standard glyph width.
     */
    unsigned int getWidth();
    /** \brief Get a font's standard glyph height.
     *
     *  Returns the standard value for the current font's glyphs. Most glyphs in
     *  the font will have this height.
     *
     *  \return The standard glyph height.
     */
    unsigned int getHeight();
    /** \brief Render the canvas onto an image buffer.
     *
     *  This function renders the given canvas on an image buffer using a specific
     *  font. The pixel format is fixed (32-bit ARGB, 8 bits for each component).
     *
     *  The required image width can be computed using
     *  caca_get_canvas_width() and caca_get_font_width(). The required
     *  height can be computed using caca_get_canvas_height() and
     *  caca_get_font_height().
     *
     *  Glyphs that do not fit in the image buffer are currently not rendered at
     *  all. They may be cropped instead in future versions.
     *
     *  \param cv The canvas to render
     *  \param buf The image buffer
     *  \param width The width (in pixels) of the image buffer
     *  \param height The height (in pixels) of the image buffer
     *  \param pitch The pitch (in bytes) of an image buffer line.
     *  \return 0 in case of success, -1 if an error occurred.
     */
    void renderCanvas(Canvas *cv, uint8_t *buf, unsigned int width,
                               unsigned int height, unsigned int pitch);
    /** \brief Get a font's list of supported glyphs.
     *
     *  This function returns the list of Unicode blocks supported by the
     *  given font. The list is a zero-terminated list of indices. Here is
     *  an example:
     *
     *  \code
     *  {
     *     0x0000, 0x0080,   // Basic latin: A, B, C, a, b, c
     *     0x0080, 0x0100,   // Latin-1 supplement: "A, 'e, ^u
     *     0x0530, 0x0590,   // Armenian
     *     0x0000, 0x0000,   // END
     *  };
     *  \endcode
     *
     *  \return The list of Unicode blocks supported by the font.
     */
    uint32_t const *getBlocks();

 private:
    caca_font *font;
};

__class Dither
{
 public:
    /** \brief Create an internal dither object.
     *
     *  Create a dither structure from its coordinates (depth, width, height and
     *  pitch) and pixel mask values. If the depth is 8 bits per pixel, the mask
     *  values are ignored and the colour palette should be set using the
     *  caca_set_dither_palette() function. For depths greater than 8 bits per
     *  pixel, a zero alpha mask causes the alpha values to be ignored.
     *
     *  \param bpp Bitmap depth in bits per pixel.
     *  \param w Bitmap width in pixels.
     *  \param h Bitmap height in pixels.
     *  \param pitch Bitmap pitch in bytes.
     *  \param rmask Bitmask for red values.
     *  \param gmask Bitmask for green values.
     *  \param bmask Bitmask for blue values.
     *  \param amask Bitmask for alpha values.
     */
    Dither(unsigned int bpp, unsigned int w, unsigned int h, unsigned int pitch,
           unsigned int rmask, unsigned int gmask, unsigned int bmask, unsigned int amask);
    ~Dither();

    /** \brief Set the palette of an 8bpp dither object.
     *
     *  Set the palette of an 8 bits per pixel bitmap. Values should be between
     *  0 and 4095 (0xfff).
     *
     *  \param r Array of 256 red values.
     *  \param g Array of 256 green values.
     *  \param b Array of 256 blue values.
     *  \param a Array of 256 alpha values.
     */
    void setPalette(uint32_t r[], uint32_t g[],
                    uint32_t b[], uint32_t a[]);
    /** \brief Set the brightness of a dither object.
     *
     *  Set the brightness of dither.
     *
     *  \param brightness brightness value.
     */
    void setBrightness(float brightness);
    /** \brief Set the gamma of a dither object.
     *
     *  Set the gamma of the given dither object. A negative value causes
     *  colour inversion.
     *
     *  \param gamma Gamma value.
     */
    void setGamma(float gamma);
    /** \brief Set the contrast of a dither object.
     *
     *  Set the contrast of dither.
     *
     *  \param contrast contrast value.
     */
    void setContrast(float contrast);
    /** \brief Set dither antialiasing
     *
     *  Tell the renderer whether to antialias the dither. Antialiasing smoothens
     *  the rendered image and avoids the commonly seen staircase effect.
     *  - \c "none": no antialiasing.
     *  - \c "prefilter" or \c "default": simple prefilter antialiasing. This
     *    is the default value.
     *
     *  \param str A string describing the antialiasing method that will be used
     *         for the dithering.
     */
    void setAntialias(char const *str);
    /** \brief Get available antialiasing methods
     *
     *  Return a list of available antialiasing methods for a given dither. The
     *  list is a NULL-terminated array of strings, interleaving a string
     *  containing the internal value for the antialiasing method to be used with
     *  caca_set_dither_antialias(), and a string containing the natural
     *  language description for that antialiasing method.
     *
     *  \return An array of strings.
     */
    char const *const * getAntialiasList();
    /** \brief Choose colours used for dithering
     *
     *  Tell the renderer which colours should be used to render the
     *  bitmap. Valid values for \c str are:
     *  - \c "mono": use light gray on a black background.
     *  - \c "gray": use white and two shades of gray on a black background.
     *  - \c "8": use the 8 ANSI colours on a black background.
     *  - \c "16": use the 16 ANSI colours on a black background.
     *  - \c "fullgray": use black, white and two shades of gray for both the
     *    characters and the background.
     *  - \c "full8": use the 8 ANSI colours for both the characters and the
     *    background.
     *  - \c "full16" or \c "default": use the 16 ANSI colours for both the
     *    characters and the background. This is the default value.
     *
     *  \param str A string describing the colour set that will be used
     *         for the dithering.
     */
    void setColor(char const *str);
    /** \brief Get available colour modes
     *
     *  Return a list of available colour modes for a given dither. The list
     *  is a NULL-terminated array of strings, interleaving a string containing
     *  the internal value for the colour mode, to be used with
     *  caca_set_dither_color(), and a string containing the natural
     *  language description for that colour mode.
     */
    char const *const * getColorList();
    /** \brief Choose characters used for dithering
     *
     *  Tell the renderer which characters should be used to render the
     *  dither. Valid values for \c str are:
     *  - \c "ascii" or \c "default": use only ASCII characters. This is the
     *    default value.
     *  - \c "shades": use Unicode characters "U+2591 LIGHT SHADE", "U+2592
     *    MEDIUM SHADE" and "U+2593 DARK SHADE". These characters are also
     *    present in the CP437 codepage available on DOS and VGA.
     *  - \c "blocks": use Unicode quarter-cell block combinations. These
     *    characters are only found in the Unicode set.
     *
     *  \param str A string describing the characters that need to be used
     *         for the dithering.
     */
    void setCharset(char const *str);
    /** \brief Get available dither character sets
     *
     *  Return a list of available character sets for a given dither. The list
     *  is a NULL-terminated array of strings, interleaving a string containing
     *  the internal value for the character set, to be used with
     *  caca_set_dither_charset(), and a string containing the natural
     *  language description for that character set.
     */
    char const *const * getCharsetList();
    /** \brief Set dithering algorithm
     *
     *  Tell the renderer which dithering algorithm should be used. Dithering is
     *  necessary because the picture being rendered has usually far more colours
     *  than the available palette. Valid values for \c str are:
     *  - \c "none": no dithering is used, the nearest matching colour is used.
     *  - \c "ordered2": use a 2x2 Bayer matrix for dithering.
     *  - \c "ordered4": use a 4x4 Bayer matrix for dithering.
     *  - \c "ordered8": use a 8x8 Bayer matrix for dithering.
     *  - \c "random": use random dithering.
     *  - \c "fstein": use Floyd-Steinberg dithering. This is the default value.
     *
     *  \param str A string describing the algorithm that needs to be used
     *         for the dithering.
     */
    void setMode(char const *str);
    /** \brief Get dithering algorithms
     *
     *  Return a list of available dithering algorithms for a given dither. The
     *  list is a NULL-terminated array of strings, interleaving a string
     *  containing the internal value for the dithering algorithm, to be used
     *  with caca_set_dither_dithering(), and a string containing the natural
     *  language description for that algorithm.
     *
     *  \return An array of strings.
     */
    char const *const * getModeList();
    /** \brief Dither a bitmap on the canvas.
     *
     *  Dither a bitmap at the given coordinates. The dither can be of any size
     *  and will be stretched to the text area.
     *
     *  \param cv A handle to the libcaca canvas.
     *  \param x X coordinate of the upper-left corner of the drawing area.
     *  \param y Y coordinate of the upper-left corner of the drawing area.
     *  \param w Width of the drawing area.
     *  \param h Height of the drawing area.
     *  \param v Bitmap's pixels.
     */
    void Bitmap(Canvas *cv, int x, int y, int w, int h, void *v);

 private:
    caca_dither *dither;
};

__class Canvas
{
    friend class Caca;
    friend class Dither;
    friend class Font;
 public:
    /** \brief Initialise a \e libcaca canvas.
     *
     *  Initialise internal \e libcaca structures and the backend that will
     *  be used for subsequent graphical operations. It must be the first
     *  \e libcaca function to be called in a function. caca_free_canvas()
     *  should be called at the end of the program to free all allocated resources.
     *
     *  Both the cursor and the canvas' handle are initialised at the top-left
     *  corner.
     */
    Canvas();
    /** \brief Initialise a \e libcaca canvas.
    *
    *  Initialise internal \e libcaca structures and the backend that will
    *  be used for subsequent graphical operations. It must be the first
    *  \e libcaca function to be called in a function. caca_free_canvas()
    *  should be called at the end of the program to free all allocated resources.
    *
    *  Both the cursor and the canvas' handle are initialised at the top-left
    *  corner.
    *
    *  \param width The desired canvas width
    *  \param height The desired canvas height
    */
    Canvas(int width, int height);
    ~Canvas();

    /** \brief Resize a canvas.
     *
     *  Set the canvas' width and height, in character cells.
     *
     *  The contents of the canvas are preserved to the extent of the new
     *  canvas size. Newly allocated character cells at the right and/or at
     *  the bottom of the canvas are filled with spaces.
     *
     *  If as a result of the resize the cursor coordinates fall outside the
     *  new canvas boundaries, they are readjusted. For instance, if the
     *  current X cursor coordinate is 11 and the requested width is 10, the
     *  new X cursor coordinate will be 10.
     *
     *  It is an error to try to resize the canvas if an output driver has
     *  been attached to the canvas using caca_create_display(). You need to
     *  remove the output driver using caca_free_display() before you can change
     *  the canvas size again. However, the caca output driver can cause a
     *  canvas resize through user interaction. See the caca_event() documentation
     *  for more about this.
     *
     *  \param width The desired canvas width.
     *  \param height The desired canvas height.
     */
    void setSize(unsigned int width, unsigned int height);
    /** \brief Get the canvas width.
     *
     *  Return the current canvas' width, in character cells.
     *
     *  \return The canvas width.
     */
    unsigned int getWidth(void);
    /** \brief Get the canvas height.
     *
     *  Returns the current canvas' height, in character cells.
     *
     *  \return The canvas height.
     */
    unsigned int getHeight(void);
    /** \brief Get the text attribute at the given coordinates.
     *
     *  Get the internal \e libcaca attribute value of the character at the
     *  given coordinates. The attribute value has 32 significant bits,
     *  organised as follows from MSB to LSB:
     *  - 3 bits for the background alpha
     *  - 4 bits for the background red component
     *  - 4 bits for the background green component
     *  - 3 bits for the background blue component
     *  - 3 bits for the foreground alpha
     *  - 4 bits for the foreground red component
     *  - 4 bits for the foreground green component
     *  - 3 bits for the foreground blue component
     *  - 4 bits for the bold, italics, underline and blink flags
     *
     *  If the coordinates are outside the canvas boundaries, the current
     *  attribute is returned.
     *
     *  \param x X coordinate.
     *  \param y Y coordinate.
     *  \return The requested attribute.
     */
    uint32_t getAttr(int x, int y);
    /** \brief Set the default character attribute.
     *
     *  Set the default character attribute for drawing. Attributes define
     *  foreground and background colour, transparency, bold, italics and
     *  underline styles, as well as blink. String functions such as
     *  caca_printf() and graphical primitive functions such as caca_draw_line()
     *  will use this attribute.
     *
     *  The value of \e attr is either:
     *  - a 32-bit integer as returned by caca_get_attr(), in which case it
     *    also contains colour information,
     *  - a combination (bitwise OR) of style values (\e CACA_UNDERLINE,
     *    \e CACA_BLINK, \e CACA_BOLD and \e CACA_ITALICS), in which case
     *    setting the attribute does not modify the current colour information.
     *
     *  To retrieve the current attribute value, use caca_get_attr(-1,-1).
     *
     *  \param attr The requested attribute value.
     *  \return This function always returns 0.
     */
    int setAttr(uint32_t attr);
    /** \brief Set the default colour pair for text (ANSI version).
     *
     *  Set the default ANSI colour pair for text drawing. String functions such
     *  as caca_printf() and graphical primitive functions such as caca_draw_line()
     *  will use these attributes.
     *
     *  Color values are those defined in caca.h, such as CACA_RED
     *  or CACA_TRANSPARENT.
     *
     *  If an error occurs, -1 is returned and \b errno is set accordingly:
     *  - \c EINVAL At least one of the colour values is invalid.
     *
     *  \param fg The requested ANSI foreground colour.
     *  \param bg The requested ANSI background colour.
     *  \return 0 in case of success, -1 if an error occurred.
     */
    int setColorANSI(uint8_t fg, uint8_t bg);
    /** \brief Set the default colour pair for text (truecolor version).
     *
     *  Set the default ARGB colour pair for text drawing. String functions such
     *  as caca_printf() and graphical primitive functions such as caca_draw_line()
     *  will use these attributes.
     *
     *  Colors are 16-bit ARGB values, each component being coded on 4 bits. For
     *  instance, 0xf088 is solid dark cyan (A=15 R=0 G=8 B=8), and 0x8fff is
     *  white with 50% alpha (A=8 R=15 G=15 B=15).
     *
     *  \param fg The requested ARGB foreground colour.
     *  \param bg The requested ARGB background colour.
     *  \return This function always returns 0.
     */
    int setColorARGB(unsigned int fg, unsigned int bg);
    void Printf(int x, int y , char const * format, ...);
    /** \brief Print an ASCII or Unicode character.
     *
     *  Print an ASCII or Unicode character at the given coordinates, using
     *  the default foreground and background colour values.
     *
     *  If the coordinates are outside the canvas boundaries, nothing is printed.
     *  If a fullwidth Unicode character gets overwritten, its remaining visible
     *  parts are replaced with spaces. If the canvas' boundaries would split the
     *  fullwidth character in two, a space is printed instead.
     *
     *  The behaviour when printing non-printable characters or invalid UTF-32
     *  characters is undefined. To print a sequence of bytes forming an UTF-8
     *  character instead of an UTF-32 character, use the caca_put_str() function.
     *
     *  This function returns the width of the printed character. If it is a
     *  fullwidth character, 2 is returned. Otherwise, 1 is returned.
     *
     *  \param x X coordinate.
     *  \param y Y coordinate.
     *  \param ch The character to print.
     */
    void putChar(int x, int y, uint32_t ch);
    /** \brief Get the Unicode character at the given coordinates.
     *
     *  Get the ASCII or Unicode value of the character at the given
     *  coordinates. If the value is less or equal to 127 (0x7f),
     *  the character can be printed as ASCII. Otherise, it must be handled
     *  as a UTF-32 value.
     *
     *  If the coordinates are outside the canvas boundaries, a space (0x20)
     *  is returned.
     *
     *  A special exception is when CACA_MAGIC_FULLWIDTH is returned. This
     *  value is guaranteed not to be a valid Unicode character, and indicates
     *  that the character at the left of the requested one is a fullwidth
     *  character.
     *
     *  \param x X coordinate.
     *  \param y Y coordinate.
     *  \return The Unicode character at the given coordinates.
     */
    uint32_t getChar(int x, int y);
    /** \brief Print a string.
     *
     *  Print an UTF-8 string at the given coordinates, using the default
     *  foreground and background values. The coordinates may be outside the
     *  canvas boundaries (eg. a negative Y coordinate) and the string will
     *  be cropped accordingly if it is too long.
     *
     *  See caca_put_char() for more information on how fullwidth characters
     *  are handled when overwriting each other or at the canvas' boundaries.
     *
     *  This function returns the number of cells printed by the string. It is
     *  not the number of characters printed, because fullwidth characters
     *  account for two cells.
     *
     *  \param x X coordinate.
     *  \param y Y coordinate.
     *  \param str The string to print.
     */
    void putStr(int x, int y, char *str);
    /** \brief Clear the canvas.
     *
     *  Clear the canvas using the current foreground and background colours.
     */
    void Clear(void);
    /** \brief Blit a canvas onto another one.
     *
     *  Blit a canvas onto another one at the given coordinates.
     *  An optional mask canvas can be used.
     *
     *  \param x X coordinate.
     *  \param y Y coordinate.
     *  \param c1 The source canvas.
     *  \param c2 The mask canvas.
     */
    void Blit(int x, int y, Canvas* c1, Canvas* c2);
    /** \brief Invert a canvas' colours.
     *
     *  Invert a canvas' colours (black becomes white, red becomes cyan, etc.)
     *  without changing the characters in it.
     */
    void Invert();
    /** \brief Flip a canvas horizontally.
     *
     *  Flip a canvas horizontally, choosing characters that look like the
     *  mirrored version wherever possible. Some characters will stay
     *  unchanged by the process, but the operation is guaranteed to be
     *  involutive: performing it again gives back the original canvas.
     */
    void Flip();
    /** \brief Flip a canvas vertically.
     *
     *  Flip a canvas vertically, choosing characters that look like the
     *  mirrored version wherever possible. Some characters will stay
     *  unchanged by the process, but the operation is guaranteed to be
     *  involutive: performing it again gives back the original canvas.
     */
    void Flop();
    /** \brief Rotate a canvas.
     *
     *  Apply a 180-degree transformation to a canvas, choosing characters
     *  that look like the upside-down version wherever possible. Some
     *  characters will stay unchanged by the process, but the operation is
     *  guaranteed to be involutive: performing it again gives back the
     *  original canvas.
     */
    void Rotate180();
    /** \brief Rotate a canvas, 90 degrees counterclockwise.
     *
     *  Apply a 90-degree transformation to a canvas, choosing characters
     *  that look like the rotated version wherever possible. Characters cells
     *  are rotated two-by-two. Some characters will stay unchanged by the
     *  process, some others will be replaced by close equivalents. Fullwidth
     *  characters at odd horizontal coordinates will be lost. The operation is
     *  not guaranteed to be reversible at all.
     *
     *  Note that the width of the canvas is divided by two and becomes the
     *  new height. Height is multiplied by two and becomes the new width. If
     *  the original width is an odd number, the division is rounded up.
     */
    void RotateLeft();
    /** \brief Rotate a canvas, 90 degrees counterclockwise.
     *
     *  Apply a 90-degree transformation to a canvas, choosing characters
     *  that look like the rotated version wherever possible. Characters cells
     *  are rotated two-by-two. Some characters will stay unchanged by the
     *  process, some others will be replaced by close equivalents. Fullwidth
     *  characters at odd horizontal coordinates will be lost. The operation is
     *  not guaranteed to be reversible at all.
     *
     *  Note that the width of the canvas is divided by two and becomes the
     *  new height. Height is multiplied by two and becomes the new width. If
     *  the original width is an odd number, the division is rounded up.
     */
    void RotateRight();
    /** \brief Draw a line on the canvas using the given character.
     *
     *  \param x1 X coordinate of the first point.
     *  \param y1 Y coordinate of the first point.
     *  \param x2 X coordinate of the second point.
     *  \param y2 Y coordinate of the second point.
     *  \param ch UTF-32 character to be used to draw the line.
     */
    void drawLine(int x1, int y1, int x2, int y2, uint32_t ch);
    /** \brief Draw a polyline.
     *
     *  Draw a polyline on the canvas using the given character and coordinate
     *  arrays. The first and last points are not connected, hence in order to
     *  draw a polygon you need to specify the starting point at the end of the
     *  list as well.
     *
     *  \param x Array of X coordinates. Must have \p n + 1 elements.
     *  \param y Array of Y coordinates. Must have \p n + 1 elements.
     *  \param n Number of lines to draw.
     *  \param ch UTF-32 character to be used to draw the lines.
     */
    void drawPolyline(int const x[], int const y[], int n, uint32_t ch);
    /** \brief Draw a thin line on the canvas, using ASCII art.
     *
     *  \param x1 X coordinate of the first point.
     *  \param y1 Y coordinate of the first point.
     *  \param x2 X coordinate of the second point.
     *  \param y2 Y coordinate of the second point.
     */
    void drawThinLine(int x1, int y1, int x2, int y2);
    /** \brief Draw an ASCII art thin polyline.
     *
     *  Draw a thin polyline on the canvas using the given coordinate arrays and
     *  with ASCII art. The first and last points are not connected, so in order
     *  to draw a polygon you need to specify the starting point at the end of
     *  the list as well.
     *
     *  \param x Array of X coordinates. Must have \p n + 1 elements.
     *  \param y Array of Y coordinates. Must have \p n + 1 elements.
     *  \param n Number of lines to draw.
     */
    void drawThinPolyline(int const x[], int const y[], int n);
    /** \brief Draw a circle on the canvas using the given character.
     *
     *  \param x Center X coordinate.
     *  \param y Center Y coordinate.
     *  \param r Circle radius.
     *  \param ch UTF-32 character to be used to draw the circle outline.
     */
    void drawCircle(int x, int y, int r, uint32_t ch);
    /** \brief Draw an ellipse on the canvas using the given character.
     *
     *  \param x Center X coordinate.
     *  \param y Center Y coordinate.
     *  \param d1 Ellipse X radius.
     *  \param d2 Ellipse Y radius.
     *  \param ch UTF-32 character to be used to draw the ellipse outline.
     */
    void drawEllipse(int x, int y, int d1, int d2, uint32_t ch);
    /** \brief Draw a thin ellipse on the canvas.
     *
     *  \param x Center X coordinate.
     *  \param y Center Y coordinate.
     *  \param d1 Ellipse X radius.
     *  \param d2 Ellipse Y radius.
     */
    void drawThinEllipse(int x, int y, int d1, int d2);
    /** \brief Fill an ellipse on the canvas using the given character.
     *
     *  \param x Center X coordinate.
     *  \param y Center Y coordinate.
     *  \param d1 Ellipse X radius.
     *  \param d2 Ellipse Y radius.
     *  \param ch UTF-32 character to be used to fill the ellipse.
     */
    void fillEllipse(int x, int y, int d1, int d2, uint32_t ch);
    /** \brief Draw a box on the canvas using the given character.
     *
     *  \param x X coordinate of the upper-left corner of the box.
     *  \param y Y coordinate of the upper-left corner of the box.
     *  \param w Width of the box.
     *  \param h Height of the box.
     *  \param ch UTF-32 character to be used to draw the box.
     */
    void drawBox(int x, int y, int w, int h, uint32_t ch);
    /** \brief Draw a box on the canvas using the given character.
     *
     *  \param x X coordinate of the upper-left corner of the box.
     *  \param y Y coordinate of the upper-left corner of the box.
     *  \param w Width of the box.
     *  \param h Height of the box.
     */
    void drawThinBox(int x, int y, int w, int h);
    /** \brief Draw a box on the canvas using CP437 characters.
     *
     *  \param x X coordinate of the upper-left corner of the box.
     *  \param y Y coordinate of the upper-left corner of the box.
     *  \param w Width of the box.
     *  \param h Height of the box.
     */
    void drawCP437Box(int x, int y, int w, int h);
    /** \brief Fill a box on the canvas using the given character.
     *
     *  \param x X coordinate of the upper-left corner of the box.
     *  \param y Y coordinate of the upper-left corner of the box.
     *  \param w Width of the box.
     *  \param h Height of the box.
     *  \param ch UTF-32 character to be used to fill the box.
     */
    void fillBox(int x, int y, int w, int h, uint32_t ch);
    /** \brief Draw a triangle on the canvas using the given character.
     *
     *  \param x1 X coordinate of the first point.
     *  \param y1 Y coordinate of the first point.
     *  \param x2 X coordinate of the second point.
     *  \param y2 Y coordinate of the second point.
     *  \param x3 X coordinate of the third point.
     *  \param y3 Y coordinate of the third point.
     *  \param ch UTF-32 character to be used to draw the triangle.
     */
    void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t ch);
    /** \brief Draw a thin triangle on the canvas.
     *
     *  \param x1 X coordinate of the first point.
     *  \param y1 Y coordinate of the first point.
     *  \param x2 X coordinate of the second point.
     *  \param y2 Y coordinate of the second point.
     *  \param x3 X coordinate of the third point.
     *  \param y3 Y coordinate of the third point.
     */
    void drawThinTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    /** \brief Fill a triangle on the canvas using the given character.
     *
     *  \param x1 X coordinate of the first point.
     *  \param y1 Y coordinate of the first point.
     *  \param x2 X coordinate of the second point.
     *  \param y2 Y coordinate of the second point.
     *  \param x3 X coordinate of the third point.
     *  \param y3 Y coordinate of the third point.
     *  \param ch UTF-32 character to be used to fill the triangle.
     */
    void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t ch);
    /** \brief Fill a triangle on the canvas using an arbitrary-sized texture.
     *
     *  This function fails if one or both the canvas are missing
     *
     *  \param coords The coordinates of the triangle (3{x,y})
     *  \param tex    The handle of the canvas texture.
     *  \param uv     The coordinates of the texture (3{u,v})
     */
    void fillTriangleTextured(int coords[6], Canvas *tex, float uv[6]);
    /** \brief Set a canvas' new boundaries.
     *
     *  Set new boundaries for a canvas. This function can be used to crop a
     *  canvas, to expand it or for combinations of both actions. All frames
     *  are affected by this function.
     *
     *  If an error occurs, -1 is returned and \b errno is set accordingly:
     *  - \c EINVAL Specified width or height is invalid.
     *  - \c EBUSY The canvas is in use by a display driver and cannot be resized.
     *  - \c ENOMEM Not enough memory for the requested canvas size. If this
     *    happens, the canvas handle becomes invalid and should not be used.
     *
     *  \param x X coordinate of the top-left corner.
     *  \param y Y coordinate of the top-left corner.
     *  \param w The width of the cropped area.
     *  \param h The height of the cropped area.
     *  \return 0 in case of success, -1 if an error occurred.
     */
    int setBoundaries(caca_canvas_t *, int x, int y, unsigned int w, unsigned int h);
    /** \brief Get the number of frames in a canvas.
     *
     *  Return the current canvas' frame count.
     *
     *  \return The frame count
     */
    unsigned int getFrameCount();
    /** \brief Activate a given canvas frame.
     *
     *  Set the active canvas frame. All subsequent drawing operations will
     *  be performed on that frame. The current painting context set by
     *  caca_set_attr() is inherited.
     *
     *  If the frame index is outside the canvas' frame range, nothing happens.
     *
     *  If an error occurs, -1 is returned and \b errno is set accordingly:
     *  - \c EINVAL Requested frame is out of range.
     *
     *  \param id The canvas frame to activate
     *  \return 0 in case of success, -1 if an error occurred.
     */
    int setFrame(unsigned int id);
    /** \brief Add a frame to a canvas.
     *
     *  Create a new frame within the given canvas. Its contents and attributes
     *  are copied from the currently active frame.
     *
     *  The frame index indicates where the frame should be inserted. Valid
     *  values range from 0 to the current canvas frame count. If the frame
     *  index is greater than or equals the current canvas frame count, the new
     *  frame is appended at the end of the canvas. If the frame index is less
     *  than zero, the new frame is inserted at index 0.
     *
     *  The active frame does not change, but its index may be renumbered due
     *  to the insertion.
     *
     *  If an error occurs, -1 is returned and \b errno is set accordingly:
     *  - \c ENOMEM Not enough memory to allocate new frame.
     *
     *  \param id The index where to insert the new frame
     *  \return 0 in case of success, -1 if an error occurred.
     */
    int createFrame(unsigned int id);
    /** \brief Remove a frame from a canvas.
     *
     *  Delete a frame from a given canvas.
     *
     *  The frame index indicates the frame to delete. Valid values range from
     *  0 to the current canvas frame count minus 1. If the frame index is
     *  greater than or equals the current canvas frame count, the last frame
     *  is deleted.
     *
     *  If the active frame is deleted, frame 0 becomes the new active frame.
     *  Otherwise, the active frame does not change, but its index may be
     *  renumbered due to the deletion.
     *
     *  If an error occurs, -1 is returned and \b errno is set accordingly:
     *  - \c EINVAL Requested frame is out of range, or attempt to delete the
     *    last frame of the canvas.
     *
     *  \param id The index of the frame to delete
     *  \return 0 in case of success, -1 if an error occurred.
     */
    int freeFrame(unsigned int id);

    /** \brief Get available import formats
     *
     *  Return a list of available import formats. The list is a NULL-terminated
     *  array of strings, interleaving a string containing the internal value for
     *  the import format, to be used with caca_import_canvas(), and a string
     *  containing the natural language description for that import format.
     *
     *  \return An array of strings.
     */
    char const * const * getImportList(void);
    /** \brief Import a memory buffer into a canvas
     *
     *  Import a memory buffer into the given libcaca canvas's current
     *  frame. The current frame is resized accordingly and its contents are
     *  replaced with the imported data.
     *
     *  Valid values for \c format are:
     *  - \c "": attempt to autodetect the file format.
     *  - \c "caca": import native libcaca files.
     *  - \c "text": import ASCII text files.
     *  - \c "ansi": import ANSI files.
     *  - \c "utf8": import UTF-8 files with ANSI colour codes.
     *  - \c "bin": import BIN files.
     *
     *  The number of bytes read is returned. If the file format is valid, but
     *  not enough data was available, 0 is returned.
     *
     *  If an error occurs, -1 is returned and \b errno is set accordingly:
     *  - \c ENOMEM Not enough memory to allocate canvas.
     *  - \c EOVERFLOW Importing data caused a value overflow.
     *  - \c EINVAL Invalid format requested.
     *
     *  \param buf A memory area containing the data to be loaded into the canvas.
     *  \param len The size in bytes of the memory area.
     *  \param fmt A string describing the input format.
     *  \return The number of bytes read, or 0 if there was not enough data,
     *  or -1 if an error occurred.
     */
    long int importFromMemory(void const *buf, size_t len, char const *fmt);
    /** \brief Import a file into a canvas
     *
     *  Import a file into the given libcaca canvas's current frame. The
     *  current frame is resized accordingly and its contents are replaced
     *  with the imported data.
     *
     *  Valid values for \c format are:
     *  - \c "": attempt to autodetect the file format.
     *  - \c "caca": import native libcaca files.
     *  - \c "text": import ASCII text files.
     *  - \c "ansi": import ANSI files.
     *  - \c "utf8": import UTF-8 files with ANSI colour codes.
     *  - \c "bin": import BIN files.
     *
     *  The number of bytes read is returned. If the file format is valid, but
     *  not enough data was available, 0 is returned.
     *
     *  If an error occurs, -1 is returned and \b errno is set accordingly:
     *  - \c ENOSYS File access is not implemented on this system.
     *  - \c ENOMEM Not enough memory to allocate canvas.
     *  - \c EINVAL Invalid format requested.
     *  caca_import_file() may also fail and set \b errno for any of the
     *  errors specified for the routine fopen().
     *
     *  \param file The name of the file to load.
     *  \param fmt A string describing the input format.
     *  \return The number of bytes read, or 0 if there was not enough data,
     *  or -1 if an error occurred.
     */
    long int importFromFile(char const *file, char const *fmt);
    /** \brief Get available export formats
     *
     *  Return a list of available export formats. The list is a NULL-terminated
     *  array of strings, interleaving a string containing the internal value for
     *  the export format, to be used with caca_export_memory(), and a string
     *  containing the natural language description for that export format.
     *
     *  \return An array of strings.
     */
    char const * const * getExportList(void);
    /** \brief Export a canvas into a foreign format.
     *
     *  This function exports a libcaca canvas into various foreign formats such
     *  as ANSI art, HTML, IRC colours, etc. The returned pointer should be passed
     *  to free() to release the allocated storage when it is no longer needed.
     *
     *  Valid values for \c format are:
     *  - \c "caca": export native libcaca files.
     *  - \c "ansi": export ANSI art (CP437 charset with ANSI colour codes).
     *  - \c "html": export an HTML page with CSS information.
     *  - \c "html3": export an HTML table that should be compatible with
     *    most navigators, including textmode ones.
     *  - \c "irc": export UTF-8 text with mIRC colour codes.
     *  - \c "ps": export a PostScript document.
     *  - \c "svg": export an SVG vector image.
     *  - \c "tga": export a TGA image.
     *  - \c "troff": export a troff source.
     *
     *  \param fmt A string describing the requested output format.
     *  \param len A pointer to a size_t where the number of allocated bytes
     *         will be written.
     */
    void *exportToMemory(char const *fmt, size_t *len);

    static int Rand(int min, int max);
    /** \brief Return the \e libcaca version.
     *
     *  Return a read-only string with the \e libcaca version information.
     *
     *  \return The \e libcaca version information.
     */
    static char const * getVersion();

 protected:
    caca_canvas_t *get_caca_canvas_t();

 private:
    caca_canvas_t *cv;
};

__class Event
{
    friend class Caca;
 public:
    enum caca_event_type
        {
            CACA_EVENT_NONE =          0x0000, /**< No event. */

            CACA_EVENT_KEY_PRESS =     0x0001, /**< A key was pressed. */
            CACA_EVENT_KEY_RELEASE =   0x0002, /**< A key was released. */
            CACA_EVENT_MOUSE_PRESS =   0x0004, /**< A mouse button was pressed. */
            CACA_EVENT_MOUSE_RELEASE = 0x0008, /**< A mouse button was released. */
            CACA_EVENT_MOUSE_MOTION =  0x0010, /**< The mouse was moved. */
            CACA_EVENT_RESIZE =        0x0020, /**< The window was resized. */

            CACA_EVENT_ANY =           0xffff  /**< Bitmask for any event. */
        } type;

 protected:
    caca_event_t e;
};

__class Caca
{
 public:
    Caca();
   /** \brief Attach a caca graphical context to a caca canvas.
    *
    *  Create a graphical context using device-dependent features (ncurses for
    *  terminals, an X11 window, a DOS command window...) that attaches to a
    *  libcaca canvas. Everything that gets drawn in the libcaca canvas can
    *  then be displayed by the libcaca driver.
    *
    *  If no caca canvas is provided, a new one is created. Its handle can be
    *  retrieved using caca_get_canvas() and it is automatically destroyed when
    *  caca_free_display() is called.
    *
    *  Note that in order to achieve maximum Unicode compatibility, the driver
    *  initialisation code may temporarily change the program’s global LC_CTYPE
    *  locale using setlocale(). It is advised not to call LC_CTYPE-dependent
    *  functions from other threads during the call to caca_create_display().
    *  The locale settings are restored when the function returns.
    *
    *  See also caca_create_display_with_driver().
    *
    *  Throws if an error occurs.
    *
    *  \param cv The caca canvas or NULL to create a canvas automatically.
    */
    Caca(Canvas *cv);
    ~Caca();

   /** \brief Attach a caca graphical context to a caca canvas.
    *
    *  Create a graphical context using device-dependent features (ncurses for
    *  terminals, an X11 window, a DOS command window...) that attaches to a
    *  libcaca canvas. Everything that gets drawn in the libcaca canvas can
    *  then be displayed by the libcaca driver.
    *
    *  If no caca canvas is provided, a new one is created. Its handle can be
    *  retrieved using caca_get_canvas() and it is automatically destroyed when
    *  caca_free_display() is called.
    *
    *  Note that in order to achieve maximum Unicode compatibility, the driver
    *  initialisation code may temporarily change the program’s global LC_CTYPE
    *  locale using setlocale(). It is advised not to call LC_CTYPE-dependent
    *  functions from other threads during the call to caca_create_display().
    *  The locale settings are restored when the function returns.
    *
    *  See also caca_create_display_with_driver().
    *
    *  Throws if an error occurs.
    *
    *  \param cv The caca canvas or NULL to create a canvas automatically.
    */
    void Attach(Canvas *cv);
   /** \brief Detach a caca graphical context from a caca backend context.
    *
    *  Detach a graphical context from its caca backend and destroy it. The
    *  libcaca canvas continues to exist and other graphical contexts can be
    *  attached to it afterwards.
    *
    *  If the caca canvas was automatically created by caca_create_display(),
    *  it is automatically destroyed and any handle to it becomes invalid.
    *
    *  This function never fails.
    */
    void Detach();
   /** \brief Set the refresh delay.
    *
    *  Set the refresh delay in microseconds. The refresh delay is used by
    *  caca_refresh_display() to achieve constant framerate. See the
    *  caca_refresh_display() documentation for more details.
    *
    *  If the argument is zero, constant framerate is disabled. This is the
    *  default behaviour.
    *
    *  \param usec The refresh delay in microseconds.
    */
    void setDisplayTime(unsigned int usec);

   /** \brief Flush pending changes and redraw the screen.
    *
    *  Flush all graphical operations and print them to the display device.
    *  Nothing will show on the screen until this function is called.
    *
    *  If caca_set_display_time() was called with a non-zero value,
    *  caca_refresh_display() will use that value to achieve constant
    *  framerate: if two consecutive calls to caca_refresh_display() are within
    *  a time range shorter than the value set with caca_set_display_time(),
    *  the second call will be delayed before performing the screen refresh.
    *
    *  This function never fails.
    */
    void Display();
   /** \brief Get the display's average rendering time.
    *
    *  Get the average rendering time, which is the average measured time
    *  between two caca_refresh_display() calls, in microseconds. If constant
    *  framerate was activated by calling caca_set_display_time(), the average
    *  rendering time will be close to the requested delay even if the real
    *  rendering time was shorter.
    *
    *  This function never fails.
    */
    unsigned int getDisplayTime();
   /** \brief Get the display width.
    *
    *  If libcaca runs in a window, get the usable window width. This value can
    *  be used for aspect ratio calculation. If libcaca does not run in a window
    *  or if there is no way to know the font size, most drivers will assume a
    *  6x10 font is being used. Note that the units are not necessarily pixels.
    *
    *  This function never fails.
    */
    unsigned int getWidth();
   /** \brief Get the display height.
    *
    *  If libcaca runs in a window, get the usable window height. This value can
    *  be used for aspect ratio calculation. If libcaca does not run in a window
    *  or if there is no way to know the font size, assume a 6x10 font is being
    *  used. Note that the units are not necessarily pixels.
    *
    *  This function never fails.
    */
    unsigned int getHeight();
   /** \brief Set the display title.
    *
    *  If libcaca runs in a window, try to change its title. This works with
    *  the ncurses, S-Lang, OpenGL, X11 and Win32 drivers.
    *
    *  If an error occurs, -1 is returned and \b errno is set accordingly:
    *  - \c ENOSYS Display driver does not support setting the window title.
    */
    int setTitle(char const *);
   /** \brief Get the next mouse or keyboard input event.
    *
    *  Poll the event queue for mouse or keyboard events matching the event
    *  mask and return the first matching event. Non-matching events are
    *  discarded. If \c event_mask is zero, the function returns immediately.
    *
    *  The timeout value tells how long this function needs to wait for an
    *  event. A value of zero returns immediately and the function returns zero
    *  if no more events are pending in the queue. A negative value causes the
    *  function to wait indefinitely until a matching event is received.
    *
    *  If not null, \c ev will be filled with information about the event
    *  received. If null, the function will return but no information about
    *  the event will be sent.
    *
    *  This function never fails.
    *
    *  \param event_mask Bitmask of requested events.
    *  \param timeout A timeout value in microseconds, -1 for blocking behaviour
    *  \param ev A pointer to a caca_event structure, or NULL.
    *  \return 1 if a matching event was received, or 0 if the wait timeouted.
    */
    int getEvent(unsigned int event_mask, Event* ev, int timeout);
   /** \brief Return the X mouse coordinate.
    *
    *  Return the X coordinate of the mouse position last time
    *  it was detected. This function is not reliable if the ncurses or S-Lang
    *  drivers are being used, because mouse position is only detected when
    *  the mouse is clicked. Other drivers such as X11 work well.
    *
    *  This function never fails.
    *
    *  \return The X mouse coordinate.
    */
    unsigned int getMouseX();
   /** \brief Return the X mouse coordinate.
    *
    *  Return the X coordinate of the mouse position last time
    *  it was detected. This function is not reliable if the ncurses or S-Lang
    *  drivers are being used, because mouse position is only detected when
    *  the mouse is clicked. Other drivers such as X11 work well.
    *
    *  This function never fails.
    *
    *  \return The X mouse coordinate.
    */
    unsigned int getMouseY();
   /** \brief Show or hide the mouse pointer.
    *
    *  Show or hide the mouse pointer. This function works with the ncurses,
    *  S-Lang and X11 drivers.
    *
    *  \param flag 0 hides the pointer, 1 shows the system's default pointer
    *              (usually an arrow). Other values are reserved for future use.
    */
    void setMouse(int flag);

   /** \brief Return the \e libcaca version.
    *
    *  Return a read-only string with the \e libcaca version information.
    *
    *  This function never fails.
    *
    *  \return The \e libcaca version information.
    */
    static char const * getVersion();
 private:
    caca_display_t *dp;
};

#endif /* _CACA_PP_H */
