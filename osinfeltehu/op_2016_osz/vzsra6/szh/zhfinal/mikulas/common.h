#ifndef __COMMON_H__
#define __COMMON_H__

struct ReqMsg {
	int a;
	int b;
	char op;
};

struct ReplyMsg {
	int result;
};

struct GiftReq {
	char giftName[20];
	int weight;	
};

struct ResultMsg {
	char msg[10];
};


#endif
