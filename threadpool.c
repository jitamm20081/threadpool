#define LL_ADD(item,list) do{
	item->next = list;
	if (list != NULL) list->prev = item;
	list = item ;
}while(0)


#define LL_REMOVE(item,list) do{
	if (item->prev != NULL) item->prev->next = item->next;
	if (item->next != NULL) item->next->prev = item->prev;
	if (list == item) list = item->next;
	item->prev = item->next = NULL;
}
struct NJOB {
	void (*job_func)(void *arg);
	void *arg;
	
	struct NJOB *next;
	struct NJOB *prev;	
};

struct NWORKER {
	pthread_t threadid;
	int terminate;
	struct NMANAGER *pool;	
	struct NWORKER *next;
	struct NWORKER *prev;
};


struct NMANAGER {
	
	struct NJOB *jobs;
	struct NWORKER *workers;
	
	pthread_mutex_t mtx;
	pthread_con_t cond;	
};

typedef struct NMANAGER NTHREADPOOL;


//create(struct NMANAGER *pool , int numworker){
//}

void *threadcallback(void *arg){
	struct NWORKER *worker = (struct NWORKER *)arg;
	
	while (1){
		pthread_mutex_lock(worker->pool->mtx);
		while (worker->pool->jobs == NULL) {
			pthread_cond_wait(&worker->pool->cond,&worker->pool->mtx);
		
		}
		
		struct NJOB *job = worker->pool->jobs;
		if (job != NULL){
			LL_REMOVE(job, worker->pool->jobs);
		}		

		pthread_mutex_unlock(worker->pool->mtx);
		job->job_func(job);
		
	}
}

int nThreadPoolCreate(NTHREADPOOL *pool, int nWorker){
	
	if(pool == NULL ) return -1;
	memset(pool, 0, sizeof(NTHREADPOOL));
	
	if(nWorker < 1) nWorker = -1;
	
	pthread_mutex_t blank_mutex = PTHREAD_MUTEX_INITIALIZER;
	memcpy(&pool->mtx, &blank_mutex, sizeof(pthread_mutex_t));
	
	pthread_cond_t blank_cond = PTHREAD_COND_INITIALIZER;
	memcpy(&pool->mtx, &blank_cond, sizeof(pthread_cond_t);
	
	int i = 0;
	for(i = 0;i < nWorker;i++){
		struct NWORKER *worker = (struct NWORKER*)malloc(sizeof(struct NWORKER*));
		if (worker == NULL){
			perror("malloc");
			return -2;
		}
		
		memset(worker, 0, sizeof(struct NWORKER));
		worker->pool = pool;
		
		int ret = pthread_create(&worker->threadid, NULL, threadcallback, worker);
		if (ret) {
			perror("pthread_create");
			free(worker);
			return -3;
		}
		
		LL_ADD(worker, pool->workers);
	}
	return 0;
}

//destroy(struct NMANAGER *pool)

//push_task(struct NMANAGER *pool,stuct *NOJB *job)

//


