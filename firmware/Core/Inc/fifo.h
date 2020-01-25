#ifndef FIFO_H
#define FIFO_H

#define fifo_flush(incnt, outcnt)					outcnt = incnt

#define fifo_space_left(incnt, outcnt, size)		(outcnt - incnt + ((outcnt > incnt) ? 0 : size) - 1)
#define fifo_space_occupied(incnt, outcnt, size)	(incnt - outcnt + ((outcnt <= incnt) ? 0 : size))

#define fifo_in(value, buffer, incnt, size)			(buffer[incnt] = (value), (++incnt == size) ? incnt = 0 : 0)
#define fifo_out(value, buffer, outcnt, size)		(value = buffer[outcnt], (++outcnt == size) ? outcnt = 0 : 0)
#define fifo_discard(buffer, outcnt, size)			((++outcnt == size) ? outcnt = 0 : 0)
#define fifo_peek(offset, buffer, outcnt, size)		(buffer[(outcnt + offset < size) ? outcnt + offset : outcnt + offset - size])
#define fifo_revpeek(offset, buffer, incnt, size)	(buffer[(incnt - offset >= 0) ? incnt - offset : incnt - offset + size])

#define fifo_full(incnt, outcnt, size)				(incnt + 1 == outcnt || (outcnt == 0 && incnt == size - 1))
#define fifo_empty(incnt, outcnt)					(incnt == outcnt)

#endif //FIFO_H
