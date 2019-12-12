#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NAMELENGTH 20//�������PID���Ƶ���󳤶�
#define RAM 5//�涨���������еĵ���
#define Table 20//�����ܵ��½�����
#define RAMSIZE 500//�ڴ��еĿռ���

int area[RAMSIZE+1];//�ڴ�ռ��ʾ��������

struct table{
    char PID[NAMELENGTH];//����PID
    int start;//������ʼλ��
    int end;//������ֹλ��
}table[Table];//������

int x = 1;//ÿһ�����̷���Ψһ�Ľ��̱�ʶ����ֻ�Ӳ��������ڷ������в�ͬ���̵����֣���ʼֵΪ1��table[0]Ϊ��ֵ��ֹ����

int tempsum1 = 0;//������ӡ���������ڽ�tempsum��ӴӶ��ڴ�ӡ������ʱȷ����Ҫ��ӡ�����У���ʼֵΪ0

int y11,y2,sizecount;//������λ������y11��y2��ֹ����������������sizecountȷ���ڴ��С�Ƿ��㹻����ȷ�����̵���ʼλ������ֹλ��

int count = RAM;//�ڴ��еĵ�����ȷ��������������������л��Ǻ󱸶���

int TotalTime = 1;//�������г��������ʱ�䣬ȷ��ѭ�������㹻���������в���ɸ���ʱ�䣬��ʼʱ��Ϊ1

//����PCB�飬�洢������Ϣ
typedef struct pcb{
    char PID[NAMELENGTH];//������PID
    int runningtime;//����ʱ��
    int priority;//����Ȩ����ֵԽ������ȨԽ��
    int status;//״̬������̬1������̬0���󱸶����о���̬-1,����̬-2
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
    //��ͷָ�����βָ�����ʱ��Ϊ����Ϊ��
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
    PCB *p = NULL;//��ʼΪ��

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
        TotalTime += p->PCB_contents.runningtime;//ȫ�ֱ������������г��������ʱ��

        printf("���̴�С��");
        scanf("%d",&p->PCB_contents.size);

        printf("��������Ȩ��");
        scanf("%d",&p->PCB_contents.priority);

        printf("�����Ƿ�Ϊ�������̣�Ϊ��������������0��Ϊ�Ƕ�������������1��");
        scanf("%d",&p->PCB_contents.property);

        if(p->PCB_contents.property == 1){//����ǷǶ���������Ҫ����ǰ�����̽�����
                printf("������ǰ�����̵Ľ�������");
                scanf("%s",p->PCB_contents.FrontPID);
        }

        //Nextָ���
        p->Next = NULL;
    
        //�ж��ڴ��е��������Ƿ��㹻��������������������������У�������������������󱸶�����
        if(count <= 0){//��������������󱸶���

            //�ڴ����
            strcpy(table[x].PID,p->PCB_contents.PID);//��ȡPID����������

            sizecount = 1;//ȷ���ڴ��С�Ƿ��㹻����ȷ�����̵���ʼλ������ֹλ��

            for(y11=0;y11<RAMSIZE;y11++){//�״���Ӧ�㷨
                sizecount++;
                if(area[y11] != 0){//��������ռ䲻�㣬���¿�ʼ����
                    sizecount = 1;
                }

                if(sizecount == p->PCB_contents.size){//������Ӧ����
                    table[x].end = y11 + 1;//�������н��̽�����ַ
                    table[x].start = y11 - sizecount + 2;//�������н�����ʼ��ַ

                    for(y2=table[x].start;y2<=table[x].end;y2++){
                        area[y2] = x;//����������е����Ϊ����Ψһ�ı�ʶ��
                    }
                    x++;//ȫ�ֱ�����ȷ��������Ľ�����Ψһ�ı�ʶ����xֻ������
                    break;//������Ӧ������ֱ���˳�
                }
            }
            if(y11 == RAMSIZE){
                printf("\n\n����ռ��Ѿ����꣡����\n\n");
                return;
            }
            
            //����������󱸶�����
            p->PCB_contents.status = -1;
            BackupQueue->Rear->Next = p;
            BackupQueue->Rear = p;

        }

        else{//�����㹻�������������

            //�ڴ����
            strcpy(table[x].PID,p->PCB_contents.PID);//��ȡPID����������

            sizecount = 1;//ȷ���ڴ��С�Ƿ��㹻����ȷ�����̵���ʼλ������ֹλ��

            for(y11=0;y11<RAMSIZE;y11++){
                sizecount++;
                if(area[y11] != 0){//��������ռ䲻�㣬���¿�ʼ����
                    sizecount = 1;
                }
                if(sizecount == p->PCB_contents.size){//������Ӧ����
                    table[x].end = y11 + 1;//�������н��̽�����ַ
                    table[x].start = y11 - sizecount+2;//�������н�����ʼ��ַ

                    for(y2=table[x].start;y2<=table[x].end;y2++){
                        area[y2] = x;//����������е����Ϊ����Ψһ�ı�ʶ��
                    }

                    x++;//ȫ�ֱ�����ȷ��������Ľ�����Ψһ�ı�ʶ����xֻ������
                    break;//������Ӧ������ֱ���˳�
                }
            }
            if(y11 == RAMSIZE){
                printf("\n\n����ռ��Ѿ����꣡����\n\n");
                return;
            }

            //�������������������
            p->PCB_contents.status = 0;
            ReadyQueue->Rear->Next = p;
            ReadyQueue->Rear = p;
            //���������м�����̺���õ���-1
            count--;
        }
    }
    
    tempsum1 += tempsum;
    //������ӡ���������ڽ�tempsum��ӴӶ��ڴ�ӡ������ʱȷ����Ҫ��ӡ������
    printf("\n");
    
};

void SortProcess(Queue *ReadyQueue){
    //����Ϊ��ֱ���˳�������Ҫ����
    if(ReadyQueue->Front == ReadyQueue->Rear){
        return;
    }
    PCB *Head = ReadyQueue->Front;//��ͷָ��
    PCB *p, *q = NULL;
    pcb t;//�м����

    //�����������еĽ��̰������ȼ��������򣨽���ð��������һЩ���������
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

    PCB *p = ReadyQueue->Front;//�������ж�ͷָ��
    //����Ϊ��ֱ���˳�������Ҫ��ӡ
    if(ReadyQueue->Front == ReadyQueue->Rear){
        printf("����������û�н��̣�\n");
    }
    else{
        printf("\n");
        printf("������\t��������ʱ��\t��������Ȩ\t���̴�С\t����״̬\tǰ�����̽�����\n");
        while(p->Next!=NULL){
            printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\t",p->Next->PCB_contents.PID,p->Next->PCB_contents.runningtime,p->Next->PCB_contents.priority,p->Next->PCB_contents.size,p->Next->PCB_contents.status);
            if(p->Next->PCB_contents.property == 1){//����ǰ��������Ҫ��ǰ�����̵�PID��ӡ����
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

    PCB *p = BackupQueue->Front;//�󱸶��ж�ͷָ��
    //����Ϊ��ֱ���˳�������Ҫ��ӡ
    if(BackupQueue->Front == BackupQueue->Rear){
        printf("�󱸶�����û�н��̣�\n");
    }
    else{
        printf("\n");
        printf("������\t��������ʱ��\t��������Ȩ\t���̴�С\t����״̬\tǰ�����̽�����\n");
        while(p->Next!=NULL){
            printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\t",p->Next->PCB_contents.PID,p->Next->PCB_contents.runningtime,p->Next->PCB_contents.priority,p->Next->PCB_contents.size,p->Next->PCB_contents.status);
            if(p->Next->PCB_contents.property != 0){//����ǰ��������Ҫ��ǰ�����̵�PID��ӡ����
                printf("%s\t",p->Next->PCB_contents.FrontPID);
            }
            printf("\n");
            p = p->Next;
        }
    }
    printf("\n");
};

void RunProcess(Queue *ReadyQueue,Queue *BackupQueue){
    //CPU A���г���
    //����������Ϊ�գ�ֱ���˳�������Ҫ����
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

        if(ta.status == -2){//������ӵ��ǹ���Ľ�����������ֱ�����
            PCB* p = (PCB*)malloc(sizeof(PCB));
            p->PCB_contents = ta;
            p->Next = NULL;
            //��Ӳ���
            BackupQueue->Rear->Next = p;
            BackupQueue->Rear = p;
        }
        else{
            if(ta.property == 0){
                break;//��ǰ������ֱ������
            }
            if(ta.property == 1){//��ǰ������
                PCB *pt = ReadyQueue->Front;//�ھ���������Ѱ��
                while(pt->Next!=NULL){
                    if(strcmp(ta.FrontPID,pt->Next->PCB_contents.PID) != 0){
                        pt = pt->Next;
                    }//�ҵ����˳�
                    else{
                        sign = 1;//��־�ھ����������ҵ���ǰ������
                        //��ta�Ľ����ͻغ󱸶���
                        ta.status = -1;//����״̬
                        PCB* p = (PCB*)malloc(sizeof(PCB));
                        p->PCB_contents = ta;
                        p->Next = NULL;
                        //��󱸶��в���
                        BackupQueue->Rear->Next = p;
                        BackupQueue->Rear = p;
                        break;
                    }   
                }

                pt = BackupQueue->Front;//�ں󱸶�����Ѱ��
                while(pt->Next!=NULL){
                    if(strcmp(ta.FrontPID,pt->Next->PCB_contents.PID) != 0){
                        pt = pt->Next;
                    }//�ҵ����˳�
                    else{
                        sign = 2;//��־�ں󱸶������ҵ���ǰ������
                        //��ta�Ľ����ͻغ󱸶���
                        ta.status = -1;
                        PCB* p = (PCB*)malloc(sizeof(PCB));
                        p->PCB_contents = ta;
                        p->Next = NULL;
                        //��Ӳ���
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
    }

    //�������еĽ��̽����ڴ����
    for(y11=1;y11<x;y11++){
        if(strcmp(ta.PID,table[y11].PID) == 0){
            for(y2=table[y11].start;y2<=table[y11].end;y2++){
                area[y2] = 0;//����������е�ֵ��Ϊ0
            }
            break;
        }
    }
    table[y11].start = -1;//������������ʼλ�ø�Ϊ-1����־�����Ѿ�����

    //������ʽ��ʼ����
    ta.status = 1;
    ta.runningtime -= 1;
    ta.priority -= 1;
    printf("\n\nCPU A ���������еĽ��̣�\n");
    printf("������\t����ʣ������ʱ��\t��������Ȩ\t���̴�С\t����״̬\n");
    printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\n\n",ta.PID,ta.runningtime,ta.priority,ta.size,ta.status);
    if(ta.runningtime == 0){
        printf("%s���н�����\n\n",ta.PID);
    }
    
    //û��������Ľ������β
    if(ta.runningtime > 0){
        ta.status = 0;
        PCB* p = (PCB*)malloc(sizeof(PCB));
        p->PCB_contents = ta;
        p->Next = NULL;
        //���������
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;

        //��ӽ����ڴ�ռ����
        for(y2=table[y11].end;y2>=table[y11].end - ta.size + 1;y2--){
            area[y2] = y11;//����������е����Ϊ����Ψһ�ı�ʶ��
        }
        table[y11].start = table[y11].end - ta.size + 1;
        
        count--;//�ڴ����-1
    }

    //����ڴ��п��е������󱸶��в�Ϊ�գ������̴Ӻ󱸶��е����������
    while((count > 0) && (BackupQueue->Front != BackupQueue->Rear)){
        PCB *pt = BackupQueue->Front->Next;
        BackupQueue->Front = pt;
        PCB* p = (PCB*)malloc(sizeof(PCB));
        pcb t;
        t = pt->PCB_contents;

        p->PCB_contents = t;
        if(p->PCB_contents.status != -2){//����ý��̲��ǹ���״̬���Ϊ����״̬
            p->PCB_contents.status = 0;
        }
        p->Next = NULL;
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;
        //��Ӳ���
        count--;//�ڴ����-1
    }

    
    //CPU B���г���
    //����������Ϊ�գ�ֱ���˳�
    if(ReadyQueue->Front == ReadyQueue->Rear){
        printf("CPU B ��û�н�������!\n\n");
        return;
    }
    
    pcb tb;
    //һֱ���ӽ���ֱ���ҵ�û��ǰ���Ľ���
    while(1){
        int sign = 0;//��־λ������Ƿ��ҵ�ǰ����δ�ҵ�ֱ���˳�
        PCB *pb = ReadyQueue->Front->Next;
        ReadyQueue->Front= pb;//����һ������

        count++;//��һ������λ

        tb = pb->PCB_contents;

        if(tb.status == -2){//������ӵ��ǹ���Ľ�����������ֱ�����
            PCB* p = (PCB*)malloc(sizeof(PCB));
            p->PCB_contents = tb;
            p->Next = NULL;
            //��Ӳ���
            BackupQueue->Rear->Next = p;
            BackupQueue->Rear = p;
            continue;
        }
        else{
            if(tb.property == 0){
                break;//��ǰ������ֱ������
            }
            if(tb.property == 1){//��ǰ������
                PCB *pt = ReadyQueue->Front;//�ھ���������Ѱ��
                while(pt->Next!=NULL){
                    if(strcmp(tb.FrontPID,pt->Next->PCB_contents.PID) != 0){
                        pt = pt->Next;
                    }//�ҵ����˳�
                    else{
                        sign = 1;//��־�ں󱸶������ҵ���ǰ������
                        //��tb�Ľ����ͻغ󱸶���
                        tb.status = -1;//����״̬
                        PCB* p = (PCB*)malloc(sizeof(PCB));
                        p->PCB_contents = tb;
                        p->Next = NULL;
                        //��󱸶��в���
                        BackupQueue->Rear->Next = p;
                        BackupQueue->Rear = p;
                        break;
                    }   
                }
                pt = BackupQueue->Front;//�ں󱸶�����Ѱ��
                while(pt->Next!=NULL){
                    if(strcmp(tb.FrontPID,pt->Next->PCB_contents.PID) != 0){
                        pt = pt->Next;
                    }//�ҵ����˳�
                    else{
                        sign = 2;//��־�ں󱸶������ҵ���ǰ������
                        //��ta�Ľ����ͻغ󱸶���
                        tb.status = -1;
                        PCB* p = (PCB*)malloc(sizeof(PCB));
                        p->PCB_contents = tb;
                        p->Next = NULL;
                        //��Ӳ���
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
    }
    //����ý��̸ո���CPU A�����У����ô˽�����CPU B�����У�ֱ���ͻؾ�������
    if(strcmp(ta.PID,tb.PID) == 0){
        printf("CPU B ��û�н�������!\n\n");
        PCB* p = (PCB*)malloc(sizeof(PCB));
        p->PCB_contents = tb;
        p->PCB_contents.status = 0;
        p->Next = NULL;
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;
        //�ڴ���õ���-1
        count--;
        return;
    }


    //�������еĽ��̽����ڴ����
    for(y11=1;y11<x;y11++){
        if(strcmp(tb.PID,table[y11].PID) == 0){
            for(y2=table[y11].start;y2<=table[y11].end;y2++){
                area[y2] = 0;//����������е�ֵ��Ϊ0
            }
            break;
        }

    }
    table[y11].start = -1;//������������ʼλ�ø�Ϊ-1����־�����Ѿ�����

    //������ʽ��ʼ����
    tb.status = 1;
    tb.runningtime -= 1;
    tb.priority -= 1;
    printf("CPU B ���������еĽ��̣�\n");
    printf("������\t����ʣ������ʱ��\t��������Ȩ\t���̴�С\t����״̬\n");
    printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\n\n",tb.PID,tb.runningtime,tb.priority,tb.size,tb.status);
    if(tb.runningtime == 0){
        printf("%s���н�����\n\n",tb.PID);
    }

    //û��������Ľ������β
    if(tb.runningtime > 0){
        tb.status = 0;
        PCB* p = (PCB*)malloc(sizeof(PCB));
        p->PCB_contents = tb;
        p->Next = NULL;
        //���������
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;


        //��ӽ����ڴ�ռ����
        for(y2=table[y11].end;y2>=table[y11].end - tb.size + 1;y2--){
            area[y2] = y11;//����������е����Ϊ����Ψһ�ı�ʶ��
        }
        table[y11].start = table[y11].end - tb.size + 1;


        count--;//�ڴ����-1
    }
    
    //����ڴ��п��е������󱸶��в�Ϊ�գ��Ӻ󱸶��е����������
    while((count > 0) && (BackupQueue->Front != BackupQueue->Rear)){
        PCB *pt = BackupQueue->Front->Next;
        BackupQueue->Front = pt;
        PCB* p = (PCB*)malloc(sizeof(PCB));
        pcb t;
        t = pt->PCB_contents;

        p->PCB_contents = t;
        if(p->PCB_contents.status != -2){//����ý��̲��ǹ���״̬���Ϊ����״̬
            p->PCB_contents.status = 0;
        }
        p->Next = NULL;
        ReadyQueue->Rear->Next = p;
        ReadyQueue->Rear = p;
        //��Ӳ���
        count--;//�ڴ����-1
    }
}

//�����û����Ὣ���̹�����߽��
void stop(Queue *ReadyQueue,Queue *BackupQueue){
    PCB *pt;
    //�������в�Ϊ��
    if(ReadyQueue->Front != ReadyQueue->Rear){
        pt = ReadyQueue->Front;
        while(pt->Next != NULL){
            //Ѱ�ҹ���Ľ��� 
            if(pt->Next->PCB_contents.status == -2){
                fflush(stdin);//�����һ���س���
                printf("��⵽%s���̱������Ƿ���?(�������y,������������n)��",pt->Next->PCB_contents.PID);
                char c;
                scanf("%c",&c);
                if(c == 'y'){
                    printf("��ҳɹ���\n");
                    pt->Next->PCB_contents.status = 0;
                }
            }
            pt = pt->Next;
        }
    }
    //�󱸶��в�Ϊ��
        pt = BackupQueue->Front;
        while(pt->Next != NULL){
            //Ѱ�ҹ���Ľ��� 
            if(pt->Next->PCB_contents.status == -2){
                fflush(stdin);//�����һ���س���
                printf("��⵽%s���̱������Ƿ���?(�������y,������������n)��",pt->Next->PCB_contents.PID);
                char c;
                scanf("%c",&c);
                if(c == 'y'){
                    printf("��ҳɹ���\n");
                    pt->Next->PCB_contents.status = -1;
                }
            }
            pt = pt->Next;
        }
    printf("��������Ҫ����Ľ�����Ŀ��");
    int s;
    scanf("%d",&s);
    int i;
    for(i=0;i<s;i++){
        char stopp[NAMELENGTH];//��ʱ�Ŀ��ַ���
        printf("�������%d����Ҫ����Ľ���PID��",i+1);
        scanf("%s",stopp);

        //�ھ�������������Ҫ����Ľ���
        if(ReadyQueue->Front != ReadyQueue->Rear){
            pt = ReadyQueue->Front;
            while(pt->Next != NULL){
                if(strcmp(pt->Next->PCB_contents.PID,stopp) == 0){
                    pt->Next->PCB_contents.status = -2;
                    printf("����ɹ���\n");
                    break;
                }
                pt = pt->Next;
            }
        }
        //�ں󱸶���������Ҫ����Ľ���
        if(BackupQueue->Front != BackupQueue->Rear){
            pt = BackupQueue->Front;
            while(pt->Next != NULL){
                if(strcmp(pt->Next->PCB_contents.PID,stopp) == 0){
                    pt->Next->PCB_contents.status = -2;
                    printf("����ɹ���\n");
                    break;
                }
                pt = pt->Next;
            }
        }
    }
}

int main(void){
    Queue ReadyQueue;//�����������
    Queue BackupQueue;//����󱸶���
    InitQueue(&ReadyQueue);//��ʼ����������
    InitQueue(&BackupQueue);//��ʼ���󱸶���
    memset(area,0,sizeof(area));//���������������ֵ��ʼֵ��Ϊ0
    char c;//��¼�û�����
    int i,j,k;
    for(i=0;i<TotalTime;i+=1){
        fflush(stdin);//�����һ���س���
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("��%d�����С���\n",i+1);

        printf("\n�Ƿ�Ҫ�����ӽ��̣����롰y�����ӣ����롰n�������ӣ�");
        scanf("%c",&c);
            if(c == 'y'){
                InputPCBInformation(&ReadyQueue,&BackupQueue);//�����û������½���
            }

        //���������
        printf("\n������\n");
        for(y2=1;y2<=tempsum1;y2++){
            printf("%s ",table[y2].PID);
            if(table[y2].start == -1){
                printf("�Ѿ�������\n");
            }
            else{
                printf("%d %d\n",table[y2].start,table[y2].end);
            }
        }

        // //����ڴ�ռ��ʾ��������
        // for(y2=0;y2<RAMSIZE+1;y2++){
        //     printf("%d ",area[y2]);
        //     if((y2+1)%50 == 0){//��ʮ�������һ��
        //         printf("\n");
        //     }
        // }

        SortProcess(&ReadyQueue); //�Ծ��������еĽ��̽�������
        
        //��ʾĿǰ����������󱸶���
        printf("\n\n�������У�\n");
        PrintReadyQueue(&ReadyQueue);
        printf("\n�󱸶��У�\n");
        PrintBackupQueue(&BackupQueue);

        // //�����û����Ὣ���̹�����߽��
        stop(&ReadyQueue,&BackupQueue);

        // //��ʾĿǰ����������󱸶���
        // printf("\n�������У�\n");
        // PrintReadyQueue(&ReadyQueue);
        // printf("\n�󱸶��У�\n");
        // PrintBackupQueue(&BackupQueue);

        //���н���
        RunProcess(&ReadyQueue,&BackupQueue);


        // //��ʾĿǰ����������󱸶���
        // printf("\n�������У�\n");
        // PrintReadyQueue(&ReadyQueue);
        // printf("\n�󱸶��У�\n");
        // PrintBackupQueue(&BackupQueue);


        //������ʾĿǰ������ʱ���Լ�ѭ��������ûʲô�ã�����ɾ�ˣ�
        // printf("\nTotalTime:%d\ti=%d",TotalTime,i);
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

        //����������о�Ϊ�գ��������н������˳�
        if((ReadyQueue.Front == ReadyQueue.Rear) && (BackupQueue.Front == BackupQueue.Rear)){
            printf("\n\n���г������н�������");
            return 0;
        }
    }

}


/*��������3
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
/*��������4
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

/*��1�����һ����ռʽ����Ȩ�����㷨ʵ�ֶദ������ȵĳ��򣬲���ʵ���ڿɱ��������ʽ�£������״���Ӧ�㷨ʵ������ռ�ķ���ͻ��ա� 
��2��PCB���ݰ�����������/PID��Ҫ������ʱ�䣨��λʱ�䣩������Ȩ��״̬���������ԣ��������̡�ͬ�����̣�ǰ������̣��� 
��3����������������ɽ��̣�����ʱ��ӽ��̣���������Ȩ���� 
��4���Ӿ�������ѡ�������У�����Ȩ-1��Ҫ������ʱ��-1��Ҫ������ʱ��Ϊ0ʱ�������ý��̣�һ��ʱ��Ƭ�������������򣬽������ֵ��ȣ� 
��5���������������������ͬ�����̵Ĵ�����������⣬ÿ�ε��Ⱥ���ʾ������״̬�����н���Ҫ��ʾ���ĸ��������ִ�С� 
��6���涨���������ú󱸶��к͹���״̬�����ڴ��н������ڹ涨���������Զ��Ӻ󱸶��е���һ��ҵ���롣����������������У����ý�ҹ������ڽ��ƶ�������̽����������С�
��7�����ʵ��һPCB�������������С��������ʼλ�ã������״���Ӧ�㷨��������ռ䡣 
��8�����м�������ռ��С��Ԥ�����ϵͳ��ռ��С������δ�ַ�������Ŀ���ݣ���ַ�����ȡ�״̬��δ��/�ձ�Ŀ���� 
��9��������ɺ󣬻������棬�������ڿ��з����ϲ��� 
��10�������Ѻã�
*/