# PhotoCrypt
An app that encrypts images :)

### Encrypts How?
It simply takes in the filename of a 24-bit bitmap image (.bmp) and an encryption key, then applies some simple arithmetic on each sub-pixel of the image to produce an unintelligible image similar to white noise.

### Why 24-bit?
It makes the most sense and it is easy to work. 24bits equates to a byte for each colour channel. It is easier to deal with integer multiples of bytes when reading or writing in binary to files.

### Why Bitmap?
The documentation is relatively easy to follow and it is a fairly simple file type to deal with.
