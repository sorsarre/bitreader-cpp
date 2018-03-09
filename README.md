# bitreader-cpp
Bitstream reader implementation in C++

At the time of writing, there was no de-facto standard library for bitwise reading operations for C++.
Most people either resorted to ugly unreadable hand-coded bitmask masturbation or to wheel reinvention.

After witnessing more than a couple of dozens of half-arsed, ad-hoc, bug-ridden implementations
(and having taken part in writing at least what, three of those?), I thought that writing another one of
them ugly crafty home-made wheels was a good fscking idea, so here we are.

## Examples

**TBD**

## Documentation

**TBD**

## Some things that remain to be done

* Examples
* API documentation
* Emulation prevention support
* Make the brcpp::bitreader use byte source abstraction instead of raw buffers
* Some sort of abstraction for arithmetic coding reading (can be implemented on top, but that stuff is pretty common)
* IEEE-754 floats (16, 32, 64, 128 bits -- no idea though what to put the 128-bit one into)
* Fixed point aka QM.N
