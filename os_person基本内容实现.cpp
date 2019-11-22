
//����PCB��
typedef struct pcb{
    char PID[NAMELENGTH];//������
    int runningtime;//����ʱ��
    int priority;//����Ȩ����ֵԽ������ȨԽ��
    int status;//״̬������̬1������̬0������̬-1
    int property;//�������ԣ�0Ϊ�������̣�1�Ƕ�������
    int frontstatus;//0��ʾ��ǰ����û��ǰ�����̣�1��ʾ��ǰ������ǰ������
    char FrontPID[NAMELENGTH];//ǰ�����̵�PID
    int backstatus;//0��ʾ��ǰ����û�к�̽��̣�1��ʾ��ǰ�����к�̽���
    char BackPID[NAMELENGTH];//��̽��̵�PID
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
    }
    q->Front->Next = NULL;
}

//����PCB��Ϣ
void InputPCBInformation(Queue *ReadyQueue,Queue *BackupQueue){
    int j;
    PCB *p = NULL;
    printf("�������������");
    int tempsum;
    scanf("%d",&tempsum);
    for(j=0;j<tempsum;j++){
        //¼����̵������Ϣ
        p = (PCB*)malloc(sizeof(PCB));
        printf("�������%d�����̵������Ϣ��\n",j+1);
        printf("��������");
        scanf("%s",p->PCB_contents.PID);
        printf("��������ʱ�䣺");
        scanf("%d",&p->PCB_contents.runningtime);
        TotalTime += p->PCB_contents.runningtime;
        printf("��������Ȩ��");
        scanf("%d",&p->PCB_contents.priority);
        printf("�����Ƿ�Ϊ�������̣�Ϊ��������������0��Ϊ�Ƕ�������������1��");
        scanf("%d",&p->PCB_contents.property);
        if(p->PCB_contents.property == 1){
            printf("�˽����Ƿ���ǰ�����̣�û��ǰ������������0����ǰ������������1��");
            scanf("%d",&p->PCB_contents.frontstatus);
            if(p->PCB_contents.frontstatus == 1){
                printf("������ǰ�����̵Ľ�������");
                scanf("%s",p->PCB_contents.FrontPID);
            }
            printf("�˽����Ƿ��к�̽��̣�û�к�̽���������0���к�̽���������1��");
            scanf("%d",&p->PCB_contents.backstatus);
            if(p->PCB_contents.backstatus == 1){
                printf("������ǰ�����̵Ľ�������");
                scanf("%s",p->PCB_contents.BackPID);
            }
        }
        p->Next = NULL;
        //�ж��ڴ��е��������Ƿ��㹻��������������������������У�������������������󱸶�����
        count--;
        if(count < 0){
            p->PCB_contents.status = -1;
            BackupQueue->Rear->Next = p;
            BackupQueue->Rear = p;
            count++;
        }
        else{
            p->PCB_contents.status = 0;
            ReadyQueue->Rear->Next = p;
            ReadyQueue->Rear = p;
        }
    }
};

void SortProcess(Queue *ReadyQueue){
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
        printf("������\t��������ʱ��\t��������Ȩ\tǰ�����̽�����\t��̽��̽�����\n");
        while(p->Next!=NULL){
            printf("%s\t%d\t%d\t",p->Next->PCB_contents.PID,p->Next->PCB_contents.runningtime,p->Next->PCB_contents.priority);
            if(p->Next->PCB_contents.frontstatus== 1){
                printf("%s\t",p->Next->PCB_contents.FrontPID);
            }
            else{
                printf("\t");
            }
            if(p->Next->PCB_contents.backstatus == 1){
                printf("%s\t",p->Next->PCB_contents.BackPID);
            }
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
        printf("������\t��������ʱ��\t��������Ȩ\tǰ�����̽�����\t��̽��̽�����\n");
        while(p->Next!=NULL){
            printf("%s\t%d\t%d\t",p->Next->PCB_contents.PID,p->Next->PCB_contents.runningtime,p->Next->PCB_contents.priority);
            if(p->Next->PCB_contents.frontstatus== 1){
                printf("%s\t",p->Next->PCB_contents.FrontPID);
            }
            else{
                printf("\t");
            }
            if(p->Next->PCB_contents.backstatus == 1){
                printf("%s\t",p->Next->PCB_contents.BackPID);
            }
            p = p->Next;
        }
    }
    printf("\n");
};

void RunProcess(Queue *ReadyQueue){
    //��ͷ�������̳���
    PCB *p1 = ReadyQueue->Front->Next;
    ReadyQueue->Front->Next = p1->Next;
    pcb t1;
    if(ReadyQueue->Front == ReadyQueue->Rear){
        return;
    }
    t1 = p1->PCB_contents;
    t1.status = 1;
    t1.runningtime -= 1;
    t1.priority -= 1;
    printf("�������еĽ��̣�\n");
    printf("������\t����ʣ������ʱ��\t��������Ȩ\n");
    printf("%s\t%d\t%d\n",t1.PID,t1.runningtime,t1.priority);
    printf("---------------------------------------------------------------------------------\n");
    //û��������Ľ������β
    if(t1.runningtime > 0){
        t1.status = 0;
        PCB* p = (PCB*)malloc(sizeof(PCB));
        p->PCB_contents = t1;
        p->Next = NULL;
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;
    }
};


int main(void){
    Queue ReadyQueue;//�����������
    Queue BackupQueue;//����󱸶���
    InitQueue(&ReadyQueue);
    InitQueue(&BackupQueue);
    char c;//��¼�û�����
    int i,j,k;
    for(i=0;i<TotalTime;i++){
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
        printf("�Ƿ�Ҫ�����ӽ��̣����롰y�����ӣ����롰n�������ӣ�");
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
        RunProcess(&ReadyQueue);
        
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