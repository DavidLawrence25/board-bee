cmake_minimum_required(VERSION 3.24.0)

add_library(ansi ansi.cc)
add_library(json json/node.cc json/parser.cc json/structure.cc json/tokenizer.cc
            json/writer.cc)
add_library(time time/date_time.cc)
