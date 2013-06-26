Terminator daemon
==================

This is a simple c++ service which can help you keep your system clean. It serves similar purposes as
OOM killer, but is easier to configure and not so fatal.

Basically this daemon watches your system and if any process exceeds the limits, it can be
killed based on preferences in order to prevent your system from crashing
