#include "sm9.h"
#include <time.h>
// #include "debug.h"
// #include <pthread.h>
// #include <omp.h>

// #include "relic_test.h"
// #include "relic_bench.h"

// /***************性能测试代码*******************/
// #include <sys/times.h>
// #include <unistd.h>
// #include <signal.h>
// #include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define count 1000
long process_do_num;  // 每个子进程完成任务的数量
fp12_t r_arr[count];
g1_t g1_arr[count];
ep2_t Ppub_arr[count];

void test_processes_init(){
	g1_t g1;
	ep2_t Ppub;
	fp12_t r;

	g1_null(g1);
	g1_new(g1);
	g1_get_gen(g1);

	ep2_null(Ppub);
	ep2_new(Ppub);

    fp12_null(r);
	fp12_new(r);

	char x0[] = "29DBA116152D1F786CE843ED24A3B573414D2177386A92DD8F14D65696EA5E32";
	char x1[] = "9F64080B3084F733E48AFF4B41B565011CE0711C5E392CFB0AB1B6791B94C408";
	char y0[] = "41E00A53DDA532DA1A7CE027B7A46F741006E85F5CDFF0730E75C05FB4E3216D";
	char y1[] = "69850938ABEA0112B57329F447E3A0CBAD3E2FDB1A77F335E89E1408D0EF1C25";
	char z0[] = "1";
	char z1[] = "0";

	fp_read_str(Ppub->x[0], x0, strlen(x0), 16);
	fp_read_str(Ppub->x[1], x1, strlen(x1), 16);
	fp_read_str(Ppub->y[0], y0, strlen(y0), 16);
	fp_read_str(Ppub->y[1], y1, strlen(y1), 16);
	fp_read_str(Ppub->z[0], z0, strlen(z0), 16);
	fp_read_str(Ppub->z[1], z1, strlen(z1), 16);

	for (size_t i = 0; i < count; i++)
	{
		fp12_null(r_arr[i]);
		fp12_new(r_arr[i]);

		g1_null(g1_arr[i]);
		g1_new(g1_arr[i]);
        g1_copy(g1_arr[i], g1);

		ep2_null(Ppub_arr[i]);
		ep2_new(Ppub_arr[i]);
		ep2_copy(Ppub_arr[i], Ppub);
	}

    sm9_init();
}
uint8_t sign_arr[count][104];
uint8_t buf_arr[count][512];
size_t siglen[count];
// SM9_SIGN_KEY sign_key[count];
// SM9_SIGN_CTX sign_ctx[count];

void test_sign_processes_init(){
	
if (core_init() != RLC_OK) {
		core_clean();
		return 1;
	}

	if (pc_param_set_any() != RLC_OK) {
		RLC_THROW(ERR_NO_CURVE);
		core_clean();
		return 0;
	}
	
	// const char *id = "Alice";

	// for(size_t i=0;i<count;i++){
	// 	for(size_t j=0;j<104;j++)
	// 		sign_arr[i][j] = '0';
	// 	for(size_t j=0;j<512;j++)
	// 		buf_arr[i][j] = '1';
	// 	siglen[i] = i;
	// }
	


}


void run(int pid)
{
    // 计算每个子进程分配到的任务区间
    int myid = pid;
    size_t start = myid * process_do_num;
    size_t end = start + process_do_num;

    if(end > count) {
        end = count;
    }

    for (size_t i = start; i < end; i++)
    {
        //sm9_pairing_fastest(r_arr[i], Ppub_arr[i], g1_arr[i]);
		SM9_SIGN_KEY sign_key;
	 SM9_SIGN_CTX sign_ctx;
	 

	sm9_sign_init(&sign_ctx);
	sm9_sign_update(&sign_ctx, (uint8_t *)"hello world", strlen("hello world"));

	//sm9_sign_finish(&sign_ctx, &sign_key, sign_arr[i], &siglen[i]);
	
	sm9_sign_finish_precompute_step2(&sign_ctx, &sign_key, sign_arr[i], &siglen[i]);
    }

#if 0
	g1_t g1;
	ep2_t Ppub;
	fp12_t r;

	g1_null(g1);
	g1_new(g1);
	g1_get_gen(g1);

	ep2_null(Ppub);
	ep2_new(Ppub);

    fp12_null(r);
	fp12_new(r);

	char x0[] = "29DBA116152D1F786CE843ED24A3B573414D2177386A92DD8F14D65696EA5E32";
	char x1[] = "9F64080B3084F733E48AFF4B41B565011CE0711C5E392CFB0AB1B6791B94C408";
	char y0[] = "41E00A53DDA532DA1A7CE027B7A46F741006E85F5CDFF0730E75C05FB4E3216D";
	char y1[] = "69850938ABEA0112B57329F447E3A0CBAD3E2FDB1A77F335E89E1408D0EF1C25";
	char z0[] = "1";
	char z1[] = "0";

	fp_read_str(Ppub->x[0], x0, strlen(x0), 16);
	fp_read_str(Ppub->x[1], x1, strlen(x1), 16);
	fp_read_str(Ppub->y[0], y0, strlen(y0), 16);
	fp_read_str(Ppub->y[1], y1, strlen(y1), 16);
	fp_read_str(Ppub->z[0], z0, strlen(z0), 16);
	fp_read_str(Ppub->z[1], z1, strlen(z1), 16);

    for (size_t i = start; i < end; i++)
    {
        sm9_pairing_faster(r, Ppub, g1);
    }
    
#endif
    // 打印调试信息
    // printf("process-%d do %d jobs\n", pid, end-start);
    exit(100+pid);
}

int test_processes(int num_processes){

    // 计算每个线程需要完成的工作量
    process_do_num = count / num_processes;
    
    // 初始化输入
    //test_processes_init();
	test_sign_processes_init();
    int status, i;
    pid_t pid[num_processes], retpid;

    struct timeval t0, t1;

    gettimeofday(&t0, NULL);

    for (i = 0; i < num_processes; i++)
    {
        if ((pid[i] = fork()) == 0)
        {
			printf("i = %d ",1);
            run(i);
        }
    }

    // 进程同步
    i = 0;
    while ((retpid = waitpid(pid[i++], &status, 0)) > 0)
    {
        if (WIFEXITED(status)){
            // 打印调试信息
            // printf("child %d terminated normally with exit status=%d\n", retpid, WEXITSTATUS(status));
        }else{
            printf("child %d terminated abnormally\n", retpid);
        }
    }
    gettimeofday(&t1, NULL);
    printf("%d processes do %d jobs in %.2f seconds\n", num_processes, count, t1.tv_sec - t0.tv_sec + 1E-6 * (t1.tv_usec - t0.tv_usec));
    return 0;
}


int main(int argc, char *argv[]) {
	if (core_init() != RLC_OK) {
		core_clean();
		return 1;
	}

	if (pc_param_set_any() != RLC_OK) {
		RLC_THROW(ERR_NO_CURVE);
		core_clean();
		return 0;
	}

    test_processes(1);
    test_processes(2);
    test_processes(3);
    test_processes(4);
    test_processes(8);
    test_processes(12);

	core_clean();

	return 0;
}
