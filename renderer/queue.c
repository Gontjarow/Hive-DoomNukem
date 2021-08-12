#include "renderer.h"

t_queue			*get_queue(void)
{
	static t_queue	*queue = NULL;
	int				i;

	if (queue == NULL)
	{
		queue = malloc(sizeof(t_queue));
		ft_crash_if(queue == NULL, "Queue wasn't allocated!");
		queue->front = queue->array;
		queue->rear = queue->array;
		ft_memset(queue->array, -1, sizeof(queue->array));
	}
	return (queue);
}

void			print_queue(t_queue *queue)
{
	int i;
	int index1;
	int index2;

	i = 0;
	printf("Queue: {");
	while (i < MAX_SECTOR_QUEUE - 1)
		printf("%i, ", queue->array[i++]);
	index1 = (queue->front - queue->array) / sizeof(queue->front);
	index2 = (queue->rear - queue->array) / sizeof(queue->rear);
	printf("%i} (Front %i, Rear %i)\n", queue->array[i], index1, index2);
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

void			queue_pop(void)
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
