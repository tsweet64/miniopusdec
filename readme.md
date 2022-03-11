# Miniopusdec

This is a very simple implementation of `libopusfile`'s decoding functions. It's probably not super useful for anything the reference tools can't do, I made this mostly to experiment with learning C++ and how to use external libraries (and opus in particular since I like it). It prints the file's metadata and outputs raw 16le PCM to a file specified. It also prints the bitrate as it decodes.

With a named pipe, you can play audio with it! Hint: Use the `mkfifo` command to create a named pipe, "play" this with `aplay -f dat <yourfifo>`, then launch miniopusdec with it as the output file.
