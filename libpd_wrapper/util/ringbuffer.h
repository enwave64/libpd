/*
 *  Copyright (c) 2012 Peter Brinkmann (peter.brinkmann@gmail.com)
 *
 *  For information on usage and redistribution, and for a DISCLAIMER OF ALL
 *  WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/libpd/libpd/wiki for documentation
 *
 */

#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

/// Simple lock-free ring buffer implementation for one writer thread and one
/// consumer thread.
typedef struct ring_buffer {
    int size;
    char *buf_ptr;
    int write_idx;
    int read_idx;
    int atomic;
} ring_buffer;

/// Creates a ring buffer (returns NULL on failure).
/// Size must be multiple of 256.
ring_buffer *rb_create(int size);

/// Deletes a ring buffer.
void rb_free(ring_buffer *buffer);

/// Returns the number of bytes that can currently be written; safe to be called
/// from any thread.
int rb_available_to_write(ring_buffer *buffer);

/// Returns the number of bytes that can currently be read; safe to be called
/// from any thread.
int rb_available_to_read(ring_buffer *buffer);

/// Writes bytes from n sources to the ring buffer (if the ring buffer has
/// enough space). The varargs are pairs of type (const char*, int) giving a
/// pointer to a buffer and the number of bytes to be copied. Only to be called
/// from a single writer thread.
/// Returns 0 on success.
int rb_write_to_buffer(ring_buffer *buffer, int n, ...);

/// Writes single byte value n times to the ring bufer (if the ring buffer has
/// enough space.) Only to be called from a single writer thread.
/// Returns 0 on success.
int rb_write_value_to_buffer(ring_buffer *buffer, int value, int n);

/// Reads the given number of bytes from the ring buffer to dest if the ring
/// buffer has enough data. Only to be called from a single reader thread.
/// Returns 0 on success.
int rb_read_from_buffer(ring_buffer *buffer, char *dest, int len);

/// Clears the contents of the ring buffer; safe to be called from any thread.
void rb_clear_buffer(ring_buffer *buffer);

/// Set the atomicity of the buffer. By default, the buffer is atomic and is
/// inherently thread safe. If you are only using one thread to read & write,
/// disabling this can increase performance slightly.
void rb_set_atomic(ring_buffer *buffer, int atomic);

/// Returns the atomicity of the buffer.
int rb_is_atomic(ring_buffer *buffer);

#endif
