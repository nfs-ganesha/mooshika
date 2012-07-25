
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>	//printf
#include <stdlib.h>	//malloc
#include <string.h>	//memcpy
#include <unistd.h>     //sleep

#include <infiniband/arch.h>
#include <rdma/rdma_cma.h>

#include "log.h"
#include "mooshika.h"

int main(int argc, char **argv) {


	struct sockaddr_in addr;
	msk_trans_t *trans;
        struct ibv_mr *mr;
        msk_data_t *rdata, *wdata;
        char *a;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(1235);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);


	a = malloc(1000*sizeof(char));
        rdata = malloc(sizeof(msk_data_t));
	wdata = malloc(sizeof(msk_data_t));


        trans = msk_create((struct sockaddr_storage*) &addr);
       	trans = msk_accept_one(trans);

	mr = msk_reg_mr(trans, a, 1000*sizeof(char), IBV_ACCESS_LOCAL_WRITE);

	wdata->data = mr->addr+100*sizeof(char);
        wdata->size=100*sizeof(char);
	strcpy((char*)wdata->data, "Message from server side (wait)!");
	sleep(1);
	msk_send_wait(trans, wdata, mr);
	INFO_LOG("sent data: %s", wdata->data);

        rdata->data=mr->addr;
        rdata->size=100*sizeof(char);
        msk_recv_wait(trans, &rdata, mr);
	INFO_LOG("recv'd data: %s", rdata->data);

	return 0;
}
