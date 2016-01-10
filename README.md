# nixie10
Library for driving 10-digit nixie tubes with Arduino

Currently supports two modes/methods of driving tubes:

1) Individual SN74141 controlled tubes with a 4-bit bus to each

2) 16-bit shift register (2x SN74595) -> 4x SN74141 driven tubes (this method requires 3 wires instead of 16 for a clock)

More methods coming later
