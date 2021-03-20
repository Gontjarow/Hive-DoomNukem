#include "renderer.h"

t_queue			*get_queue()
{
	static t_queue queue = {{0}, queue.array, queue.array};

	return (&queue);
}

void			queue_add(int id, int left, int right)
{
	t_queue *queue;

	queue = get_queue();

	queue->rear->id = id;
	queue->rear->left = left;
	queue->rear->right = right;
	queue->rear++;

	if (queue->rear == (queue->array + MAX_SECTOR_QUEUE))
		queue->rear = queue->array;
}

void			queue_pop()
{
	t_queue *queue;

	queue = get_queue();

	queue->front->id = -1;
	queue->front->left = -1;
	queue->front->right = -1;
	queue->front++;

	if (queue->front == (queue->array + MAX_SECTOR_QUEUE))
		queue->front = queue->array;
}
