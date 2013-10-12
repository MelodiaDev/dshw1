Introduction
============

* Homework 1 (a text editor) of Data Structure Practice course.
* Inspired by Vim and designed to be like Vim, but aimed to be a new editor.

Licensing
=========

* See the COPYING file.

Installation
============

* This package is distributed under UNIX/Linux environment.
* The easiest way is to type `./configure && make` to build, and `make install`
  to install.
* You may also add configure options. A list of configure options is printed by
  running `configure --help`.

Usage
=====

* Run `dshw1 [file name]` to start the editor.
* Use *H*/*J*/*K*/*L* or arrow keys to move, *Ctrl-H* or so to scroll half a
  screen.
* Use *D*/*Shift-D* to delete the current line and move down/up.
* Use *X*/*Shift-X* to delete the next/previous character.
* Use *O*/*Shift-O* to create a new line below/above the current line.
* Press *Ctrl-S* to save and *Ctrl-Q* to exit.
* Use */* to find a word, *Shift-/* to find a word backwards and *Ctrl-/* to
  replace.
* Use *Tab*/*Shift-Tab* to shift to the next/previous argument in command mode.
  You enter the mode when you save/find/replace.
* Press *I* to enter insert mode, and then type and move just like in those
  common text editors.
* Press *Esc*/*Ctrl-C* to exit insert/command mode.
* *No trailing new line character (LF) after the last line.*
