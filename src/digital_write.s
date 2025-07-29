# pre assumption for writing
.intel_syntax noprefix

# main function to be called
.global digital_write

# support function for the first vars
.global digital_pin_to_timer
.global digital_pin_to_bit_mask
.global digital_pin_to_port

# actual implementation
# first argument rdi, second rsi
digital_write:
	xor r8, r8
	xor r9, r9
	xor r10, r10
	call digital_pin_to_timer
	mov r8, rax
	call digital_pin_to_bit_mask
	mov r9, rax
	call digital_pin_to_port
	mov r10, rax
	cmp r10, 0
	je done
	cmp r8, 0
	je turn_off

done:
	xor rax, rax
	ret

turn_off:
	

digital_pin_to_timer:
