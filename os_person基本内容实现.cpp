#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NAMELENGTH 20//�������PID���Ƶ���󳤶�
#define RAM 5//�涨�ڴ��еĵ���
int count = RAM;//�ڴ��еĵ���
int TotalTime = 1;//�������г��������ʱ�䣬�������в���ɸ���ʱ�䣬��ʼʱ��Ϊ1

//����PCB��
typedef struct pcb{
    char PID[NAMELENGTH];//������
    int runningtime;//����ʱ��
    int priority;//����Ȩ����ֵԽ������ȨԽ��
    int status;//״̬������̬1������̬0������̬-1
    int property;//�������ԣ�0Ϊ�������̣�1�Ƕ�������
    int size;//���̴�С
    char FrontPID[NAMELENGTH];//ǰ�����̵�PID
}pcb;



typedef struct Node{
    pcb PCB_contents;//PCB����
    struct Node* Next;//PCBָ����һ��PCB��ָ��
}PCB;

//������нṹ
typedef struct Queue{
    PCB *Front;//��ͷָ��
    PCB *Rear;//��βָ��
}Queue;

//��ʼ������
void InitQueue(Queue *q){
    q->Front = q->Rear = (PCB*)malloc(sizeof(PCB));
    if((!q->Front)||(!q->Rear)){
        printf("��������ʧ�ܣ�");
        return;
    }
    q->Front->Next = NULL;
}

//����PCB��Ϣ
void InputPCBInformation(Queue *ReadyQueue,Queue *BackupQueue){
    int j;
    PCB *p = NULL;
    printf("�������������");
    int tempsum;//���������
    scanf("%d",&tempsum);
    for(j=0;j<tempsum;j++){
        //¼����̵������Ϣ
        p = (PCB*)malloc(sizeof(PCB));
        printf("\n�������%d�����̵������Ϣ��\n",j+1);
        printf("��������");
        scanf("%s",p->PCB_contents.PID);
        printf("��������ʱ�䣺");
        scanf("%d",&p->PCB_contents.runningtime);
        TotalTime += p->PCB_contents.runningtime;
        printf("���̴�С��");
        scanf("%d",&p->PCB_contents.size);
        printf("��������Ȩ��");
        scanf("%d",&p->PCB_contents.priority);
        printf("�����Ƿ�Ϊ�������̣�Ϊ��������������0��Ϊ�Ƕ�������������1��");
        scanf("%d",&p->PCB_contents.property);
        if(p->PCB_contents.property == 1){
                printf("������ǰ�����̵Ľ�������");
                scanf("%s",p->PCB_contents.FrontPID);
        }
        p->Next = NULL;
    
        //�ж��ڴ��е��������Ƿ��㹻��������������������������У�������������������󱸶�����
        if(count <= 0){
            p->PCB_contents.status = -1;
            BackupQueue->Rear->Next = p;
            BackupQueue->Rear = p;
        }
        else{
            p->PCB_contents.status = 0;
            ReadyQueue->Rear->Next = p;
            ReadyQueue->Rear = p;
            count--;
        }
    }
};

void SortProcess(Queue *ReadyQueue){
    if(ReadyQueue->Front == ReadyQueue->Rear){
        return;
    }
    PCB *Head = ReadyQueue->Front;
    PCB *p, *q = NULL;
    pcb t;//�м����

    //�����������еĽ��̰������ȼ���������
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
//��ӡ���������еĽ���
void PrintReadyQueue(Queue *ReadyQueue){
    PCB *p = ReadyQueue->Front;
    if(ReadyQueue->Front == ReadyQueue->Rear){
        printf("����������û�н��̣�\n");
    }
    else{
        printf("\n");
        printf("������\t��������ʱ��\t��������Ȩ\t���̴�С\t����״̬\tǰ�����̽�����\n");
        while(p->Next!=NULL){
            printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\t",p->Next->PCB_contents.PID,p->Next->PCB_contents.runningtime,p->Next->PCB_contents.priority,p->Next->PCB_contents.size,p->Next->PCB_contents.status);
            if(p->Next->PCB_contents.property == 1){
                printf("%s\t",p->Next->PCB_contents.FrontPID);
            }
            printf("\n");
            p = p->Next;
        }
    }
    printf("\n");
};
//��ӡ�󱸶����еĽ���
void PrintBackupQueue(Queue *BackupQueue){
    PCB *p = BackupQueue->Front;
    if(BackupQueue->Front == BackupQueue->Rear){
        printf("�󱸶�����û�н��̣�\n");
    }
    else{
        printf("\n");
        printf("������\t��������ʱ��\t��������Ȩ\t���̴�С\t����״̬\tǰ�����̽�����\n");
        while(p->Next!=NULL){
            printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\t",p->Next->PCB_contents.PID,p->Next->PCB_contents.runningtime,p->Next->PCB_contents.priority,p->Next->PCB_contents.size,p->Next->PCB_contents.status);
            if(p->Next->PCB_contents.property != 0){
                printf("%s\t",p->Next->PCB_contents.FrontPID);
            }
            printf("\n");
            p = p->Next;
        }
    }
    printf("\n");
};

void RunProcess(Queue *ReadyQueue,Queue *BackupQueue){
    // //CPU A���г���
    // //����������Ϊ�գ�ֱ���˳�
    if(ReadyQueue->Front == ReadyQueue->Rear){
        printf("CPU A ��û�н�������!\n\n");
        return;
    }
    pcb ta;
    //һֱ���ӽ���ֱ���ҵ�û��ǰ���Ľ���
    while(1){
        int sign = 0;//��־λ������Ƿ��ҵ�ǰ����δ�ҵ�ֱ���˳�
        PCB *pa = ReadyQueue->Front->Next;
        ReadyQueue->Front= pa;//����һ������
        count++;//��һ������λ

        ta = pa->PCB_contents;
        if(ta.property == 0){
            break;//��ǰ������ֱ������
        }
        if(ta.property == 1){//��ǰ������
            PCB *pt = ReadyQueue->Front;//�ھ���������Ѱ��
            while(pt->Next!=NULL){
                if(strcmp(ta.FrontPID,pt->Next->PCB_contents.PID) != 0){
                    pt = pt->Next;
                }
                else{
                    pt->Next->PCB_contents.priority = pt->Next->PCB_contents.priority + ta.priority + ta.runningtime + 1;
                    sign = 1;//��־�ھ����������ҵ���ǰ������
                    //��ta�Ľ����ͻغ󱸶���
                    ta.status = 0;
                    PCB* p = (PCB*)malloc(sizeof(PCB));
                    p->PCB_contents = ta;
                    p->Next = NULL;
                    BackupQueue->Rear->Next = p;
                    BackupQueue->Rear = p;
                    break;
                }   
            }
            pt = BackupQueue->Front;//�ں󱸶�����Ѱ��
            while(pt->Next!=NULL){
                if(strcmp(ta.FrontPID,pt->Next->PCB_contents.PID) != 0){
                    pt = pt->Next;
                }
                else{
                    sign = 2;//��־�ں󱸶������ҵ���ǰ������
                    //��ta�Ľ����ͻغ󱸶���
                    ta.status = 0;
                    PCB* p = (PCB*)malloc(sizeof(PCB));
                    p->PCB_contents = ta;
                    p->Next = NULL;
                    BackupQueue->Rear->Next = p;
                    BackupQueue->Rear = p;
                    break;
                }    
            }
        }
        if(sign == 0){//��ǰ�����̵���û�ҵ���Ҳ��������
            break;
        }
    }

    ta.status = 1;
    ta.runningtime -= 1;
    ta.priority -= 1;
    printf("CPU A ���������еĽ��̣�\n");
    printf("������\t����ʣ������ʱ��\t��������Ȩ\t���̴�С\t����״̬\n");
    printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\n\n",ta.PID,ta.runningtime,ta.priority,ta.size,ta.status);
    
    //û��������Ľ������β
    if((ta.status == 1) && (ta.runningtime > 0)){
        ta.status = 0;
        PCB* p = (PCB*)malloc(sizeof(PCB));
        p->PCB_contents = ta;
        p->Next = NULL;
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;
        count--;
    }

    //����ڴ��п��е������Ӻ󱸶��е����������
    while((count > 0) && (BackupQueue->Front != BackupQueue->Rear)){
        PCB *pt = BackupQueue->Front->Next;
        BackupQueue->Front = pt;
        PCB* p = (PCB*)malloc(sizeof(PCB));
        pcb t;
        t = pt->PCB_contents;
        p->PCB_contents = t;
        p->PCB_contents.status = 0;
        p->Next = NULL;
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;
        count--;
    }

    
    //CPU B����
    //����������Ϊ�գ�ֱ���˳�
    if(ReadyQueue->Front == ReadyQueue->Rear){
        printf("CPU B ��û�н�������!\n\n");
        return;
    }
    
    pcb tb;
    while(1){
        int sign = 0;//��־λ������Ƿ��ҵ�ǰ����δ�ҵ�ֱ���˳�
        PCB *pb = ReadyQueue->Front->Next;
        ReadyQueue->Front= pb;//����һ������
        count++;//��һ������λ

        tb = pb->PCB_contents;
        if(tb.property == 0){
            break;//��ǰ������ֱ������
        }
        if(tb.property == 1){//��ǰ������
            PCB *pt = ReadyQueue->Front;//�ھ���������Ѱ��
            while(pt->Next!=NULL){
                if(strcmp(tb.FrontPID,pt->Next->PCB_contents.PID) != 0){
                    pt = pt->Next;
                }
                else{
                    pt->Next->PCB_contents.priority = pt->Next->PCB_contents.priority + tb.priority + tb.runningtime + 1;
                    sign = 1;//��־�ھ����������ҵ���ǰ������
                    //��ta�Ľ����ͻغ󱸶���
                    tb.status = 0;
                    PCB* p = (PCB*)malloc(sizeof(PCB));
                    p->PCB_contents = tb;
                    p->Next = NULL;
                    BackupQueue->Rear->Next = p;
                    BackupQueue->Rear = p;
                    break;
                }   
            }
            pt = BackupQueue->Front;//�ں󱸶�����Ѱ��
            while(pt->Next!=NULL){
                if(strcmp(tb.FrontPID,pt->Next->PCB_contents.PID) != 0){
                    pt = pt->Next;
                }
                else{
                    sign = 2;//��־�ں󱸶������ҵ���ǰ������
                    //��ta�Ľ����ͻغ󱸶���
                    tb.status = 0;
                    PCB* p = (PCB*)malloc(sizeof(PCB));
                    p->PCB_contents = tb;
                    p->Next = NULL;
                    BackupQueue->Rear->Next = p;
                    BackupQueue->Rear = p;
                    break;
                }    
            }
        }
        if(sign == 0){//��ǰ�����̵���û�ҵ���Ҳ��������
            break;
        }
    }
    if(strcmp(ta.PID,tb.PID) == 0){
        printf("CPU B ��û�н�������!\n\n");
        PCB* p = (PCB*)malloc(sizeof(PCB));
        p->PCB_contents = tb;
        p->PCB_contents.status = 0;
        p->Next = NULL;
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;
        count--;
        return;
    }

    tb.status = 1;
    tb.runningtime -= 1;
    tb.priority -= 1;
    printf("CPU B ���������еĽ��̣�\n");
    printf("������\t����ʣ������ʱ��\t��������Ȩ\t���̴�С\t����״̬\n");
    printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\n\n",tb.PID,tb.runningtime,tb.priority,tb.size,tb.status);

    if((tb.status == 1) && (tb.runningtime > 0)){
        tb.status = 0;
        PCB* p = (PCB*)malloc(sizeof(PCB));
        p->PCB_contents = tb;
        p->Next = NULL;
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;
        count--;
    }
    
    //����ڴ��п��е������Ӻ󱸶��е����������
    while((count > 0) && (BackupQueue->Front != BackupQueue->Rear)){
        PCB *pt = BackupQueue->Front->Next;
        BackupQueue->Front = pt;
        PCB* p = (PCB*)malloc(sizeof(PCB));
        pcb t;
        t = pt->PCB_contents;
        p->PCB_contents = t;
        p->PCB_contents.status = 0;
        p->Next = NULL;
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;
        count--;
    }
}

int main(void){
    Queue ReadyQueue;//�����������
    Queue BackupQueue;//����󱸶���
    InitQueue(&ReadyQueue);
    InitQueue(&BackupQueue);
    char c;//��¼�û�����
    int i,j,k;
    for(i=0;i<TotalTime;i+=1){
        // //���û�ѡ����Խ�����״̬�Ľ��̽��
        // if(BackupQueue.Front != BackupQueue.Rear){
        //     printf("��⵽�й���״̬�Ľ��̣��Ƿ��ң����롰y��ѡ����̲���ң����롰n������ң�");
        //     scanf("%c",&c);
        //     if(c == 'y'){

        //     }
        //     else{
        //         break;
        //     }
        // }
        //���û����ӽ���
        fflush(stdin);//�����һ���س���
        printf("\n�Ƿ�Ҫ�����ӽ��̣����롰y�����ӣ����롰n�������ӣ�");
        scanf("%c",&c);
            if(c == 'y'){
                InputPCBInformation(&ReadyQueue,&BackupQueue);
                //TotalTime;
            }
        //�Զ����еĽ��̽�������
        SortProcess(&ReadyQueue);
        printf("----------------------------------------------------------------------------");
        //��ʾĿǰ����������󱸶���
        printf("\n�������У�\n");
        PrintReadyQueue(&ReadyQueue);
        printf("\n�󱸶��У�\n");
        PrintBackupQueue(&BackupQueue);

        //ģ�����н���
        RunProcess(&ReadyQueue,&BackupQueue);
        printf("\nTotalTime:%d\ti=%d",TotalTime,i);
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        if((ReadyQueue.Front == ReadyQueue.Rear) && (BackupQueue.Front == BackupQueue.Rear)){
            printf("\n\n���г������н�������");
            return 0;
        }
    }

}


/*��������
y
3
one
5
6
0
two
6
7
0
three
4
5
0
*/ 

/*��������2
y
7
one
5
6
1
sdfdsf
two
6
7
1
dsfdsfds
three
4
5
0
four
8
9
0
five
9
10
0
six
3
6
0
seven
8
10
0
*/
/*��������3
y
8
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
10
30
4
0
four
2
10
8
1
five
five
5
100
10
1
eight
six
7
20
11
0
seven
9
70
9
0
eight
1
120
2
1
seven
*/
/*��������4
y
3
one
5
20
8
1
two
two
8
10
6
0
three
4
5
5
0
*/