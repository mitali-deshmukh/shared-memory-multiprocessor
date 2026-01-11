# Multi-Processor Memory Access Simulator

This project implements a C-based simulator for modeling memory access behavior across multiple processors. It evaluates average memory access time under different request distributions and contention scenarios. The simulator is designed to study how processor requests interact over time and how denial and retry behavior affects overall performance.



## Overview

- Simulates multiple processors issuing memory access requests.
- Supports uniform and non-uniform request distributions.
- Tracks access attempts, denials, retries, and timing.
- Computes average access time across simulation cycles.
- Designed for performance and systems analysis coursework.



## Core Concepts

- Each processor repeatedly issues memory requests.
- Requests may be granted or denied depending on contention.
- Denied processors retry after backoff.
- Simulation runs for a fixed maximum number of cycles.
- Final output reports average memory access time.

