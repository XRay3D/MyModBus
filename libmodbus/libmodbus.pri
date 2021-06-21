INCLUDEPATH = \
    $$PWD \
    $$PWD/src/ \

HEADERS += \
#    $$PWD/libmodbus.h \
    $$PWD/modbusutility.h \
    $$PWD/mymodbus.h \
#    $$PWD/src/modbus-private.h \
#    $$PWD/src/modbus-rtu-private.h \
#    $$PWD/src/modbus-rtu.h \
##    $$PWD/src/modbus-tcp-private.h \
##    $$PWD/src/modbus-tcp.h \
#    $$PWD/src/modbus-version.h \
#    $$PWD/src/modbus-version.h.in \
#    $$PWD/src/modbus.h \

SOURCES += \
#    $$PWD/libmodbus.cpp \
    $$PWD/modbusutility.cpp \
    $$PWD/mymodbus.cpp \
#    $$PWD/src/modbus-data.c \
#    $$PWD/src/modbus-rtu.c \
##    $$PWD/src/modbus-tcp.c \
#    $$PWD/src/modbus.c \


DISTFILES += \
    $$PWD/doc/asciidoc.conf \
    $$PWD/doc/libmodbus.txt \
    $$PWD/doc/modbus_close.txt \
    $$PWD/doc/modbus_connect.txt \
    $$PWD/doc/modbus_flush.txt \
    $$PWD/doc/modbus_free.txt \
    $$PWD/doc/modbus_get_byte_from_bits.txt \
    $$PWD/doc/modbus_get_byte_timeout.txt \
    $$PWD/doc/modbus_get_float.txt \
    $$PWD/doc/modbus_get_float_abcd.txt \
    $$PWD/doc/modbus_get_float_badc.txt \
    $$PWD/doc/modbus_get_float_cdab.txt \
    $$PWD/doc/modbus_get_float_dcba.txt \
    $$PWD/doc/modbus_get_header_length.txt \
    $$PWD/doc/modbus_get_response_timeout.txt \
    $$PWD/doc/modbus_get_slave.txt \
    $$PWD/doc/modbus_get_socket.txt \
    $$PWD/doc/modbus_mapping_free.txt \
    $$PWD/doc/modbus_mapping_new.txt \
    $$PWD/doc/modbus_mapping_new_start_address.txt \
    $$PWD/doc/modbus_mask_write_register.txt \
    $$PWD/doc/modbus_new_rtu.txt \
    $$PWD/doc/modbus_new_tcp.txt \
    $$PWD/doc/modbus_new_tcp_pi.txt \
    $$PWD/doc/modbus_read_bits.txt \
    $$PWD/doc/modbus_read_input_bits.txt \
    $$PWD/doc/modbus_read_input_registers.txt \
    $$PWD/doc/modbus_read_registers.txt \
    $$PWD/doc/modbus_receive.txt \
    $$PWD/doc/modbus_receive_confirmation.txt \
    $$PWD/doc/modbus_reply.txt \
    $$PWD/doc/modbus_reply_exception.txt \
    $$PWD/doc/modbus_report_slave_id.txt \
    $$PWD/doc/modbus_rtu_get_rts.txt \
    $$PWD/doc/modbus_rtu_get_rts_delay.txt \
    $$PWD/doc/modbus_rtu_get_serial_mode.txt \
    $$PWD/doc/modbus_rtu_set_custom_rts.txt \
    $$PWD/doc/modbus_rtu_set_rts.txt \
    $$PWD/doc/modbus_rtu_set_rts_delay.txt \
    $$PWD/doc/modbus_rtu_set_serial_mode.txt \
    $$PWD/doc/modbus_send_raw_request.txt \
    $$PWD/doc/modbus_set_bits_from_byte.txt \
    $$PWD/doc/modbus_set_bits_from_bytes.txt \
    $$PWD/doc/modbus_set_byte_timeout.txt \
    $$PWD/doc/modbus_set_debug.txt \
    $$PWD/doc/modbus_set_error_recovery.txt \
    $$PWD/doc/modbus_set_float.txt \
    $$PWD/doc/modbus_set_float_abcd.txt \
    $$PWD/doc/modbus_set_float_badc.txt \
    $$PWD/doc/modbus_set_float_cdab.txt \
    $$PWD/doc/modbus_set_float_dcba.txt \
    $$PWD/doc/modbus_set_response_timeout.txt \
    $$PWD/doc/modbus_set_slave.txt \
    $$PWD/doc/modbus_set_socket.txt \
    $$PWD/doc/modbus_strerror.txt \
    $$PWD/doc/modbus_tcp_accept.txt \
    $$PWD/doc/modbus_tcp_listen.txt \
    $$PWD/doc/modbus_tcp_pi_accept.txt \
    $$PWD/doc/modbus_tcp_pi_listen.txt \
    $$PWD/doc/modbus_write_and_read_registers.txt \
    $$PWD/doc/modbus_write_bit.txt \
    $$PWD/doc/modbus_write_bits.txt \
    $$PWD/doc/modbus_write_register.txt \
    $$PWD/doc/modbus_write_registers.txt \
    $$PWD/help.txt \
    $$PWD/src/win32/modbus.dll.manifest.in \
    $$PWD/tests/README.md \
    $$PWD/tests/unit-tests.sh \
    $$PWD/tests/bandwidth-client.c \
    $$PWD/tests/bandwidth-server-many-up.c \
    $$PWD/tests/bandwidth-server-one.c \
    $$PWD/tests/random-test-client.c \
    $$PWD/tests/random-test-server.c \
    $$PWD/tests/unit-test-client.c \
    $$PWD/tests/unit-test-server.c \
    $$PWD/tests/version.c \
    $$PWD/tests/unit-test.h \
    $$PWD/tests/unit-test.h.in \



