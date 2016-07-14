Introduction
============

The most popular software is IDA Pro by Hex-Rays SA. This commercial software is the de-facto standard in binary analysis, but it is quite expensive, which make it not very accessible for a hobby-researcher. There is the option to use an older version for free (v5.0), but this version is for non-commercial use only and lacks many features and there is also no support.

An alternative to IDA Pro is an open-source project called Radare2, which is free to use and already offers quiet many professional features. More are added continuously or are getting improved, because the software is actively maintained and developed and gets more and more recognition in the security scene. You can take a look at a presentation from REcon 2015 about the differences about IDA Pro and Radare2 [here](http://radare.org/get/recon2015.pdf).

Why we want to use binary analysis tools?

Well, the reasons can be very different, for example:
- good intention: 
	- malware analysis
	- finding and fixing bugs
	- reverse engineer software, where the source-code was lost
	- Hacking competitions (CTF)
- bad intention: 
	- cracking or reverse engineer protected commercial software
	- adding harmful components to software
	- break into systems
- something in between:
	- curiosity

This tutorial will give you some exercises and sample solutions for basic situations to learn to get used to the usage of Radare2 and/or IDA Pro (v5.0 - free version). It is based on a learning by doing approach, and although the exercises are not hard to solve, you may get used to work with the software and therefore can move to more complex tasks by reading the official documentation and complete more advanced tutorials.

What this tutorial is NOT about:
- showing all the features the software is capable of
- learning assembly language
- learning about security vulnerabilities in software
- doing something illegal (all the binaries provided are free and have their source-code included)

_Note: because there may be changes in the build environment, it is possible that the binaries are changed comparing to earlier versions of this tutorial._