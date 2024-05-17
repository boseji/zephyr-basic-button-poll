# Copyright (c) 2020 Abhijit Bose <https://boseji.com>
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(basic_button_poll)

target_sources(app PRIVATE src/main.c)
