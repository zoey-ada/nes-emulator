#pragma once

enum class AddressingMode
{
	zero_page,
	zero_page_x_read,
	zero_page_x_write,
	zero_page_y_read,
	zero_page_y_write,

	absolute,
	absolute_x_read,
	absolute_x_write,
	absolute_y_read,
	absolute_y_write,

	indirect,
	indirect_x_read,
	indirect_x_write,
	indirect_y_read,
	indirect_y_write,

	relative,
	immediate,
	accumulator,
	implicit,
};
