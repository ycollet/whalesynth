pkg_check_modules(JACK jack)
if (NOT JACK_FOUND)
  message(FATAL_ERROR "can't find jack library")
endif(NOT JACK_FOUND)

