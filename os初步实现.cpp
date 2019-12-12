#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NAMELENGTH 20//定义进程PID名称的最大长度
#define RAM 5//规定就绪队列中的道数
#define Table 20//最多接受的新进程数
#define RAMSIZE 500//内存中的空间数

int area[RAMSIZE+1];//内存空间表示矩阵数组

struct table{
    char PID[NAMELENGTH];//进程PID
    int start;//进程起始位置
    int end;//进程终止位置
}table[Table];//分区表

int x = 1;//每一个进程分配唯一的进程标识符，只加不减，用于分区表中不同进程的区分，初始值为1，table[0]为空值禁止访问

int tempsum1 = 0;//辅助打印分区表，用于将tempsum相加从而在打印分区表时确认需要打印多少行，初始值为0

int y11,y2,sizecount;//辅助定位变量，y11，y2防止与其他变量混淆，sizecount确定内存大小是否足够，并确定进程的起始位置与终止位置

int count = RAM;//内存中的道数，确定进程输入后进入就绪队列还是后备队列

int TotalTime = 1;//定义所有程序的运行时间，确保循环次数足够，过程中有插入可更新时间，初始时间为1

//定义PCB块，存储进程信息
typedef struct pcb{
    char PID[NAMELENGTH];//进程名PID
    int runningtime;//运行时间
    int priority;//优先权，数值越大优先权越高
    int status;//状态，运行态1，就绪态0，后备队列中就绪态-1,挂起态-2
    int property;//进程属性：0为独立进程，1非独立进程
    int size;//进程大小
    char FrontPID[NAMELENGTH];//前趋进程的PID
}pcb;

typedef struct Node{
    pcb PCB_contents;//PCB内容
    struct Node* Next;//PCB指向下一个PCB的指针
}PCB;

//定义队列结构
typedef struct Queue{
    PCB *Front;//队头指针
    PCB *Rear;//队尾指针
    //队头指针与队尾指针相等时即为队列为空
}Queue;

//初始化队列
void InitQueue(Queue *q){
    q->Front = q->Rear = (PCB*)malloc(sizeof(PCB));
    if((!q->Front)||(!q->Rear)){
        printf("创建队列失败！");
        return;
    }
    q->Front->Next = NULL;
}

//输入PCB信息
void InputPCBInformation(Queue *ReadyQueue,Queue *BackupQueue){
    int j;
    PCB *p = NULL;//初始为空

    printf("请输入进程数：");
    int tempsum;//保存进程数
    scanf("%d",&tempsum);

    for(j=0;j<tempsum;j++){
        //录入进程的相关信息
        p = (PCB*)malloc(sizeof(PCB));

        printf("\n请输入第%d个进程的相关信息：\n",j+1);

        printf("进程名：");
        scanf("%s",p->PCB_contents.PID);

        printf("进程运行时间：");
        scanf("%d",&p->PCB_contents.runningtime);
        TotalTime += p->PCB_contents.runningtime;//全局变量，定义所有程序的运行时间

        printf("进程大小：");
        scanf("%d",&p->PCB_contents.size);

        printf("进程优先权：");
        scanf("%d",&p->PCB_contents.priority);

        printf("进程是否为独立进程？为独立进程请输入0，为非独立进程请输入1：");
        scanf("%d",&p->PCB_contents.property);

        if(p->PCB_contents.property == 1){//如果是非独立进程需要输入前趋进程进程名
                printf("请输入前趋进程的进程名：");
                scanf("%s",p->PCB_contents.FrontPID);
        }

        //Next指向空
        p->Next = NULL;
    
        //判断内存中道数数量是否足够，数量够将进程送入就绪队列中，数量不够将进程送入后备队列中
        if(count <= 0){//数量不够，送入后备队列

            //内存操作
            strcpy(table[x].PID,p->PCB_contents.PID);//获取PID到分区表中

            sizecount = 1;//确定内存大小是否足够，并确定进程的起始位置与终止位置

            for(y11=0;y11<RAMSIZE;y11++){//首次适应算法
                sizecount++;
                if(area[y11] != 0){//如果连续空间不足，重新开始计数
                    sizecount = 1;
                }

                if(sizecount == p->PCB_contents.size){//满足适应条件
                    table[x].end = y11 + 1;//分区表中进程结束地址
                    table[x].start = y11 - sizecount + 2;//分区表中进程起始地址

                    for(y2=table[x].start;y2<=table[x].end;y2++){
                        area[y2] = x;//将数组矩阵中的零改为进程唯一的标识符
                    }
                    x++;//全局变量，确保新输入的进程是唯一的标识符，x只增不减
                    break;//满足适应条件后直接退出
                }
            }
            if(y11 == RAMSIZE){
                printf("\n\n主存空间已经用完！！！\n\n");
                return;
            }
            
            //将进程送入后备队列中
            p->PCB_contents.status = -1;
            BackupQueue->Rear->Next = p;
            BackupQueue->Rear = p;

        }

        else{//数量足够，送入就绪队列

            //内存操作
            strcpy(table[x].PID,p->PCB_contents.PID);//获取PID到分区表中

            sizecount = 1;//确定内存大小是否足够，并确定进程的起始位置与终止位置

            for(y11=0;y11<RAMSIZE;y11++){
                sizecount++;
                if(area[y11] != 0){//如果连续空间不足，重新开始计数
                    sizecount = 1;
                }
                if(sizecount == p->PCB_contents.size){//满足适应条件
                    table[x].end = y11 + 1;//分区表中进程结束地址
                    table[x].start = y11 - sizecount+2;//分区表中进程起始地址

                    for(y2=table[x].start;y2<=table[x].end;y2++){
                        area[y2] = x;//将数组矩阵中的零改为进程唯一的标识符
                    }

                    x++;//全局变量，确保新输入的进程是唯一的标识符，x只增不减
                    break;//满足适应条件后直接退出
                }
            }
            if(y11 == RAMSIZE){
                printf("\n\n主存空间已经用完！！！\n\n");
                return;
            }

            //将进程送入就绪队列中
            p->PCB_contents.status = 0;
            ReadyQueue->Rear->Next = p;
            ReadyQueue->Rear = p;
            //就绪队列中加入进程后可用道数-1
            count--;
        }
    }
    
    tempsum1 += tempsum;
    //辅助打印分区表，用于将tempsum相加从而在打印分区表时确认需要打印多少行
    printf("\n");
    
};

void SortProcess(Queue *ReadyQueue){
    //队列为空直接退出，不需要排序
    if(ReadyQueue->Front == ReadyQueue->Rear){
        return;
    }
    PCB *Head = ReadyQueue->Front;//队头指针
    PCB *p, *q = NULL;
    pcb t;//中间变量

    //将就绪队列中的进程按照优先级进行排序（近似冒泡排序，有一些多余遍历）
    for(q = Head->Next;q->Next != NULL;q = q->Next){
        for(p = Head->Next;p->Next != NULL;p = p->Next){
            if(p->PCB_contents.priority < p->Next->PCB_contents.priority){
                t = p->PCB_contents;
                p->PCB_contents = p->Next->PCB_contents;
                p->Next->PCB_contents = t;
            }
        }
    }
};
//打印就绪队列中的进程
void PrintReadyQueue(Queue *ReadyQueue){

    PCB *p = ReadyQueue->Front;//就绪队列队头指针
    //队列为空直接退出，不需要打印
    if(ReadyQueue->Front == ReadyQueue->Rear){
        printf("就绪队列中没有进程！\n");
    }
    else{
        printf("\n");
        printf("进程名\t进程运行时间\t进程优先权\t进程大小\t进程状态\t前趋进程进程名\n");
        while(p->Next!=NULL){
            printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\t",p->Next->PCB_contents.PID,p->Next->PCB_contents.runningtime,p->Next->PCB_contents.priority,p->Next->PCB_contents.size,p->Next->PCB_contents.status);
            if(p->Next->PCB_contents.property == 1){//如有前趋进程需要将前趋进程的PID打印出来
                printf("%s\t",p->Next->PCB_contents.FrontPID);
            }
            printf("\n");
            p = p->Next;
        }
    }
    printf("\n");
};
//打印后备队列中的进程
void PrintBackupQueue(Queue *BackupQueue){

    PCB *p = BackupQueue->Front;//后备队列队头指针
    //队列为空直接退出，不需要打印
    if(BackupQueue->Front == BackupQueue->Rear){
        printf("后备队列中没有进程！\n");
    }
    else{
        printf("\n");
        printf("进程名\t进程运行时间\t进程优先权\t进程大小\t进程状态\t前趋进程进程名\n");
        while(p->Next!=NULL){
            printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\t",p->Next->PCB_contents.PID,p->Next->PCB_contents.runningtime,p->Next->PCB_contents.priority,p->Next->PCB_contents.size,p->Next->PCB_contents.status);
            if(p->Next->PCB_contents.property != 0){//如有前趋进程需要将前趋进程的PID打印出来
                printf("%s\t",p->Next->PCB_contents.FrontPID);
            }
            printf("\n");
            p = p->Next;
        }
    }
    printf("\n");
};

void RunProcess(Queue *ReadyQueue,Queue *BackupQueue){
    //CPU A运行程序
    //若就绪队列为空，直接退出，不需要运行
    if(ReadyQueue->Front == ReadyQueue->Rear){
        printf("CPU A 中没有进程运行!\n\n");
        return;
    }
    pcb ta;
    //一直出队进程直到找到没有前趋的进程
    while(1){
        int sign = 0;//标志位，检查是否找到前趋，未找到直接退出
        PCB *pa = ReadyQueue->Front->Next;
        ReadyQueue->Front= pa;//出队一个进程

        count++;//多一个空闲位

        ta = pa->PCB_contents;

        if(ta.status == -2){//如果出队的是挂起的进程则不能运行直接入队
            PCB* p = (PCB*)malloc(sizeof(PCB));
            p->PCB_contents = ta;
            p->Next = NULL;
            //入队操作
            BackupQueue->Rear->Next = p;
            BackupQueue->Rear = p;
        }
        else{
            if(ta.property == 0){
                break;//无前趋进程直接运行
            }
            if(ta.property == 1){//有前趋进程
                PCB *pt = ReadyQueue->Front;//在就绪队列中寻找
                while(pt->Next!=NULL){
                    if(strcmp(ta.FrontPID,pt->Next->PCB_contents.PID) != 0){
                        pt = pt->Next;
                    }//找到后退出
                    else{
                        sign = 1;//标志在就绪队列中找到了前趋进程
                        //将ta的进程送回后备队列
                        ta.status = -1;//更改状态
                        PCB* p = (PCB*)malloc(sizeof(PCB));
                        p->PCB_contents = ta;
                        p->Next = NULL;
                        //入后备队列操作
                        BackupQueue->Rear->Next = p;
                        BackupQueue->Rear = p;
                        break;
                    }   
                }

                pt = BackupQueue->Front;//在后备队列中寻找
                while(pt->Next!=NULL){
                    if(strcmp(ta.FrontPID,pt->Next->PCB_contents.PID) != 0){
                        pt = pt->Next;
                    }//找到后退出
                    else{
                        sign = 2;//标志在后备队列中找到了前趋进程
                        //将ta的进程送回后备队列
                        ta.status = -1;
                        PCB* p = (PCB*)malloc(sizeof(PCB));
                        p->PCB_contents = ta;
                        p->Next = NULL;
                        //入队操作
                        BackupQueue->Rear->Next = p;
                        BackupQueue->Rear = p;
                        break;
                    }    
                }
            }
            if(sign == 0){//有前趋进程但是没找到，也可以运行
                break;
            }
        }
    }

    //出队运行的进程进行内存操作
    for(y11=1;y11<x;y11++){
        if(strcmp(ta.PID,table[y11].PID) == 0){
            for(y2=table[y11].start;y2<=table[y11].end;y2++){
                area[y2] = 0;//将数组矩阵中的值改为0
            }
            break;
        }
    }
    table[y11].start = -1;//将分区表中起始位置改为-1，标志进程已经出队

    //进程正式开始运行
    ta.status = 1;
    ta.runningtime -= 1;
    ta.priority -= 1;
    printf("\n\nCPU A 中正在运行的进程：\n");
    printf("进程名\t进程剩余运行时间\t进程优先权\t进程大小\t进程状态\n");
    printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\n\n",ta.PID,ta.runningtime,ta.priority,ta.size,ta.status);
    if(ta.runningtime == 0){
        printf("%s运行结束！\n\n",ta.PID);
    }
    
    //没有运行完的进程入队尾
    if(ta.runningtime > 0){
        ta.status = 0;
        PCB* p = (PCB*)malloc(sizeof(PCB));
        p->PCB_contents = ta;
        p->Next = NULL;
        //入就绪队列
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;

        //入队进程内存空间操作
        for(y2=table[y11].end;y2>=table[y11].end - ta.size + 1;y2--){
            area[y2] = y11;//将数组矩阵中的零改为进程唯一的标识符
        }
        table[y11].start = table[y11].end - ta.size + 1;
        
        count--;//内存道数-1
    }

    //如果内存有空闲道数，后备队列不为空，将进程从后备队列调入就绪队列
    while((count > 0) && (BackupQueue->Front != BackupQueue->Rear)){
        PCB *pt = BackupQueue->Front->Next;
        BackupQueue->Front = pt;
        PCB* p = (PCB*)malloc(sizeof(PCB));
        pcb t;
        t = pt->PCB_contents;

        p->PCB_contents = t;
        if(p->PCB_contents.status != -2){//如果该进程不是挂起状态则改为就绪状态
            p->PCB_contents.status = 0;
        }
        p->Next = NULL;
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;
        //入队操作
        count--;//内存道数-1
    }

    
    //CPU B运行程序
    //若就绪队列为空，直接退出
    if(ReadyQueue->Front == ReadyQueue->Rear){
        printf("CPU B 中没有进程运行!\n\n");
        return;
    }
    
    pcb tb;
    //一直出队进程直到找到没有前趋的进程
    while(1){
        int sign = 0;//标志位，检查是否找到前趋，未找到直接退出
        PCB *pb = ReadyQueue->Front->Next;
        ReadyQueue->Front= pb;//出队一个进程

        count++;//多一个空闲位

        tb = pb->PCB_contents;

        if(tb.status == -2){//如果出队的是挂起的进程则不能运行直接入队
            PCB* p = (PCB*)malloc(sizeof(PCB));
            p->PCB_contents = tb;
            p->Next = NULL;
            //入队操作
            BackupQueue->Rear->Next = p;
            BackupQueue->Rear = p;
            continue;
        }
        else{
            if(tb.property == 0){
                break;//无前趋进程直接运行
            }
            if(tb.property == 1){//有前趋进程
                PCB *pt = ReadyQueue->Front;//在就绪队列中寻找
                while(pt->Next!=NULL){
                    if(strcmp(tb.FrontPID,pt->Next->PCB_contents.PID) != 0){
                        pt = pt->Next;
                    }//找到后退出
                    else{
                        sign = 1;//标志在后备队列中找到了前趋进程
                        //将tb的进程送回后备队列
                        tb.status = -1;//更改状态
                        PCB* p = (PCB*)malloc(sizeof(PCB));
                        p->PCB_contents = tb;
                        p->Next = NULL;
                        //入后备队列操作
                        BackupQueue->Rear->Next = p;
                        BackupQueue->Rear = p;
                        break;
                    }   
                }
                pt = BackupQueue->Front;//在后备队列中寻找
                while(pt->Next!=NULL){
                    if(strcmp(tb.FrontPID,pt->Next->PCB_contents.PID) != 0){
                        pt = pt->Next;
                    }//找到后退出
                    else{
                        sign = 2;//标志在后备队列中找到了前趋进程
                        //将ta的进程送回后备队列
                        tb.status = -1;
                        PCB* p = (PCB*)malloc(sizeof(PCB));
                        p->PCB_contents = tb;
                        p->Next = NULL;
                        //入队操作
                        BackupQueue->Rear->Next = p;
                        BackupQueue->Rear = p;
                        break;
                    }    
                }
            }
            if(sign == 0){//有前趋进程但是没找到，也可以运行
                break;
            }
        }
    }
    //如果该进程刚刚在CPU A中运行，则不让此进程在CPU B中运行，直接送回就绪队列
    if(strcmp(ta.PID,tb.PID) == 0){
        printf("CPU B 中没有进程运行!\n\n");
        PCB* p = (PCB*)malloc(sizeof(PCB));
        p->PCB_contents = tb;
        p->PCB_contents.status = 0;
        p->Next = NULL;
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;
        //内存可用道数-1
        count--;
        return;
    }


    //出队运行的进程进行内存操作
    for(y11=1;y11<x;y11++){
        if(strcmp(tb.PID,table[y11].PID) == 0){
            for(y2=table[y11].start;y2<=table[y11].end;y2++){
                area[y2] = 0;//将数组矩阵中的值改为0
            }
            break;
        }

    }
    table[y11].start = -1;//将分区表中起始位置改为-1，标志进程已经出队

    //进程正式开始运行
    tb.status = 1;
    tb.runningtime -= 1;
    tb.priority -= 1;
    printf("CPU B 中正在运行的进程：\n");
    printf("进程名\t进程剩余运行时间\t进程优先权\t进程大小\t进程状态\n");
    printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\n\n",tb.PID,tb.runningtime,tb.priority,tb.size,tb.status);
    if(tb.runningtime == 0){
        printf("%s运行结束！\n\n",tb.PID);
    }

    //没有运行完的进程入队尾
    if(tb.runningtime > 0){
        tb.status = 0;
        PCB* p = (PCB*)malloc(sizeof(PCB));
        p->PCB_contents = tb;
        p->Next = NULL;
        //入就绪队列
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;


        //入队进程内存空间操作
        for(y2=table[y11].end;y2>=table[y11].end - tb.size + 1;y2--){
            area[y2] = y11;//将数组矩阵中的零改为进程唯一的标识符
        }
        table[y11].start = table[y11].end - tb.size + 1;


        count--;//内存道数-1
    }
    
    //如果内存有空闲道数，后备队列不为空，从后备队列调入就绪队列
    while((count > 0) && (BackupQueue->Front != BackupQueue->Rear)){
        PCB *pt = BackupQueue->Front->Next;
        BackupQueue->Front = pt;
        PCB* p = (PCB*)malloc(sizeof(PCB));
        pcb t;
        t = pt->PCB_contents;

        p->PCB_contents = t;
        if(p->PCB_contents.status != -2){//如果该进程不是挂起状态则改为就绪状态
            p->PCB_contents.status = 0;
        }
        p->Next = NULL;
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;
        //入队操作
        count--;//内存道数-1
    }
}

//给予用户机会将进程挂起或者解挂
void stop(Queue *ReadyQueue,Queue *BackupQueue){
    PCB *pt;
    //就绪队列不为空
    if(ReadyQueue->Front != ReadyQueue->Rear){
        pt = ReadyQueue->Front;
        while(pt->Next != NULL){
            //寻找挂起的进程 
            if(pt->Next->PCB_contents.status == -2){
                fflush(stdin);//清除上一个回车符
                printf("检测到%s进程被挂起，是否解挂?(解挂输入y,继续挂起输入n)：",pt->Next->PCB_contents.PID);
                char c;
                scanf("%c",&c);
                if(c == 'y'){
                    printf("解挂成功！\n");
                    pt->Next->PCB_contents.status = 0;
                }
            }
            pt = pt->Next;
        }
    }
    //后备队列不为空
        pt = BackupQueue->Front;
        while(pt->Next != NULL){
            //寻找挂起的进程 
            if(pt->Next->PCB_contents.status == -2){
                fflush(stdin);//清除上一个回车符
                printf("检测到%s进程被挂起，是否解挂?(解挂输入y,继续挂起输入n)：",pt->Next->PCB_contents.PID);
                char c;
                scanf("%c",&c);
                if(c == 'y'){
                    printf("解挂成功！\n");
                    pt->Next->PCB_contents.status = -1;
                }
            }
            pt = pt->Next;
        }
    printf("请输入想要挂起的进程数目：");
    int s;
    scanf("%d",&s);
    int i;
    for(i=0;i<s;i++){
        char stopp[NAMELENGTH];//临时的空字符串
        printf("请输入第%d个想要挂起的进程PID：",i+1);
        scanf("%s",stopp);

        //在就绪队列中找想要挂起的进程
        if(ReadyQueue->Front != ReadyQueue->Rear){
            pt = ReadyQueue->Front;
            while(pt->Next != NULL){
                if(strcmp(pt->Next->PCB_contents.PID,stopp) == 0){
                    pt->Next->PCB_contents.status = -2;
                    printf("挂起成功！\n");
                    break;
                }
                pt = pt->Next;
            }
        }
        //在后备队列中找想要挂起的进程
        if(BackupQueue->Front != BackupQueue->Rear){
            pt = BackupQueue->Front;
            while(pt->Next != NULL){
                if(strcmp(pt->Next->PCB_contents.PID,stopp) == 0){
                    pt->Next->PCB_contents.status = -2;
                    printf("挂起成功！\n");
                    break;
                }
                pt = pt->Next;
            }
        }
    }
}

int main(void){
    Queue ReadyQueue;//定义就绪队列
    Queue BackupQueue;//定义后备队列
    InitQueue(&ReadyQueue);//初始化就绪队列
    InitQueue(&BackupQueue);//初始化后备队列
    memset(area,0,sizeof(area));//将分区数组的所有值初始值设为0
    char c;//记录用户操作
    int i,j,k;
    for(i=0;i<TotalTime;i+=1){
        fflush(stdin);//清除上一个回车符
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("第%d次运行——\n",i+1);

        printf("\n是否要新增加进程？输入“y”增加，输入“n”不增加：");
        scanf("%c",&c);
            if(c == 'y'){
                InputPCBInformation(&ReadyQueue,&BackupQueue);//允许用户输入新进程
            }

        //输出分区表
        printf("\n分区表：\n");
        for(y2=1;y2<=tempsum1;y2++){
            printf("%s ",table[y2].PID);
            if(table[y2].start == -1){
                printf("已经结束！\n");
            }
            else{
                printf("%d %d\n",table[y2].start,table[y2].end);
            }
        }

        // //输出内存空间表示矩阵数组
        // for(y2=0;y2<RAMSIZE+1;y2++){
        //     printf("%d ",area[y2]);
        //     if((y2+1)%50 == 0){//五十个数输出一行
        //         printf("\n");
        //     }
        // }

        SortProcess(&ReadyQueue); //对就绪队列中的进程进行排序
        
        //显示目前就绪队列与后备队列
        printf("\n\n就绪队列：\n");
        PrintReadyQueue(&ReadyQueue);
        printf("\n后备队列：\n");
        PrintBackupQueue(&BackupQueue);

        // //给予用户机会将进程挂起或者解挂
        stop(&ReadyQueue,&BackupQueue);

        // //显示目前就绪队列与后备队列
        // printf("\n就绪队列：\n");
        // PrintReadyQueue(&ReadyQueue);
        // printf("\n后备队列：\n");
        // PrintBackupQueue(&BackupQueue);

        //运行进程
        RunProcess(&ReadyQueue,&BackupQueue);


        // //显示目前就绪队列与后备队列
        // printf("\n就绪队列：\n");
        // PrintReadyQueue(&ReadyQueue);
        // printf("\n后备队列：\n");
        // PrintBackupQueue(&BackupQueue);


        //辅助显示目前的总体时间以及循环次数（没什么用，可以删了）
        // printf("\nTotalTime:%d\ti=%d",TotalTime,i);
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

        //如果两个队列均为空，整个运行结束，退出
        if((ReadyQueue.Front == ReadyQueue.Rear) && (BackupQueue.Front == BackupQueue.Rear)){
            printf("\n\n所有程序运行结束！！");
            return 0;
        }
    }

}


/*测试数据3
y
6
one
3
20
5
0
two
4
50
7
1
one
three
4
30
8
0
four
2
10
6
1
five
five
4
100
5
0
six
3
20
4
0

*/
/*测试数据4
y
3
one
3
20
4
1
two
two
2
10
3
0
three
4
5
5
0
*/

/*（1）设计一个抢占式优先权调度算法实现多处理机调度的程序，并且实现在可变分区管理方式下，采用首次适应算法实现主存空间的分配和回收。 
（2）PCB内容包括：进程名/PID；要求运行时间（单位时间）；优先权；状态；进程属性：独立进程、同步进程（前趋、后继）。 
（3）可以随机输入若干进程，可随时添加进程，并按优先权排序； 
（4）从就绪队首选进程运行：优先权-1；要求运行时间-1；要求运行时间为0时，撤销该进程；一个时间片结束后重新排序，进行下轮调度； 
（5）考虑两个处理机，考虑同步进程的处理机分配问题，每次调度后，显示各进程状态，运行进程要显示在哪个处理机上执行。 
（6）规定道数，设置后备队列和挂起状态。若内存中进程少于规定道数，可自动从后备队列调度一作业进入。被挂起进程入挂起队列，设置解挂功能用于将制定挂起进程解挂入就绪队列。
（7）结合实验一PCB增加所需主存大小，主存起始位置；采用首次适应算法分配主存空间。 
（8）自行假设主存空间大小，预设操作系统所占大小并构造未分分区表。表目内容：起址、长度、状态（未分/空表目）。 
（9）进程完成后，回收主存，并与相邻空闲分区合并。 
（10）界面友好；
*/