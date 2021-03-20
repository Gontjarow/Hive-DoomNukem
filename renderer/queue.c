#include "renderer.h"

t_queue			*get_queue()
{
	static t_queue queue = {{0}, queue.array, queue.array + 1};

	return (&queue);
}

static t_queue	*init_queue()
{
	t_queue *queue;

	queue = get_queue();

	queue->front = queue->array;
	queue->rear = queue->front + 1;
}

void			queue_add(int id, int left, int right)
{
	t_queue *queue;

	queue = get_queue();

	queue->rear->id = id;
	queue->rear->left = 0;
	queue->rear->right = GAME_WIN_WIDTH - 1;
	queue->rear++;

	// Note: If rear == queue+MAX_SECTOR_QUEUE
	//       set to 0 (circle back to beginning)

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

	// Note: If front == queue+MAX_SECTOR_QUEUE
	//       set to 0 (circle back to beginning)

	if (queue->front == (queue->array + MAX_SECTOR_QUEUE))
		queue->front = queue->array;
}
