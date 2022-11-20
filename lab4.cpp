#include <iostream>
#include <string>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <vector>
#include <queue>
using namespace std;

typedef struct 
{
    char ProcessName;
    int processNumber;
    int ArrivalTime;
    int ServiceTime;
    int FinishingTime;
    int TurnaroundTime;
    int priority;
    int WaitingTime;
    float NormTurn;
    int ready;
}Process;

struct MyCompartor{
    bool operator() (Process* p1, Process* p2)
    {
        if (p1->ServiceTime == p2->ServiceTime)
        {
            return p1->ArrivalTime > p2->ArrivalTime;
        }
        return p1->ServiceTime > p2->ServiceTime;
    }
};

struct MyCompartor2{
    bool operator() (Process* p1, Process* p2)
    {
        float ratio1 = (p1->WaitingTime + p1->ServiceTime)/(float)p1->ServiceTime;
        float ratio2 = (p2->WaitingTime + p2->ServiceTime)/(float)p2->ServiceTime;
        if (ratio1 == ratio2)
        {
            return p1->ArrivalTime < p2->ArrivalTime;
        }
        return ratio1 < ratio2;
    }
};

struct MyCompartor3
{
    bool operator() (Process* p1, Process* p2)
    {
        if (p1->priority == p2->priority)
        {
            return p1->ArrivalTime > p2->ArrivalTime;
        }
        return p1->priority < p2->priority;
    }
};

Process* create_Process(string ProcessName, int ArrivalTime, int ServiceTime)
{
    Process *p = (Process *)malloc(sizeof(Process));
    p->ProcessName = ProcessName.back();
    p->processNumber = 0;
    p->ready = 0;
    p->FinishingTime = 0;
    p->TurnaroundTime = 0;
    p->NormTurn = 0;
    p->WaitingTime = 0;
    p->ArrivalTime = ArrivalTime;
    p->ServiceTime = ServiceTime;
    p->priority = ServiceTime;
    return p;
}

Process * formulate_Process(string process)
{
    stringstream ss(process);
    string str[3];
    char i=0;
    while(!ss.eof())
    {
        getline(ss, str[i],',');
        i++;
    }
    return create_Process(str[0],stoi(str[1]),stoi(str[2]));
}


void printTrace(int instances,char ** output, int NoOfProcesses, Process** p, string policyname)
{
    char mypolicy[policyname.size()+1];
    strcpy(mypolicy, policyname.c_str());
    printf("%-6s", mypolicy);
    size_t i;
    for (i = 0; i <= instances; i++)
    {
        cout << (i%10) << " ";
    }
    cout << endl;
    cout << "------------------------------------------------\n";
    for (size_t i = 0; i < NoOfProcesses; i++)
    {
        cout << p[i]->ProcessName << "     ";
        for (size_t j = 0; j < instances; j++)
        {
            cout << "|" << output[i][j];
        }

        cout << "| " <<endl;
    }
    cout << "------------------------------------------------\n\n";
}

void printStats(Process** P, int NoOfProcesses, string policyname)
{
    cout << policyname;
    cout << endl;
    int count = NoOfProcesses;
    printf("Process    ");
    for (size_t i = 0; i < count; i++)
    {
       printf("|%3c  ",P[i]->ProcessName);
    }
    printf("|\n");
    printf("Arrival    ");
    for (size_t i = 0; i < count; i++)
    {
        printf("|%3d  ",P[i]->ArrivalTime);
    }
    printf("|\n");
    printf("Service    ");
    for (size_t i = 0; i < count; i++)
    {
        printf("|%3d  ",P[i]->ServiceTime);
    }
    printf("| Mean|\n");
    printf("Finish     ");
    for (size_t i = 0; i < count; i++)
    {
        printf("|%3d  ",P[i]->FinishingTime);
    }
    printf("|-----|\n");
    printf("Turnaround ");
    float TotalTurnAround = 0;
    size_t i;
    for (i = 0; i < count; i++)
    {
        P[i]->TurnaroundTime=P[i]->FinishingTime - P[i]->ArrivalTime;
        TotalTurnAround += P[i]->TurnaroundTime;
        printf("|%3d  ",P[i]->TurnaroundTime);
    }
    printf("|%5.2f|\n",(TotalTurnAround/i));
    printf("NormTurn   ");
    float TotalNormTime = 0;
    for (i = 0; i < count; i++)
    {
       P[i]->NormTurn = ((float)P[i]->TurnaroundTime / P[i]->ServiceTime);
       TotalNormTime += P[i]->NormTurn;
       printf("|%5.2f", P[i]->NormTurn);
    }
    printf("|%5.2f|\n\n", (TotalNormTime/i));
}

char ** create_outputArray(int r, int c)
{
    char** output = (char **)malloc( sizeof(char *) * r);
    for (size_t i = 0; i < r; i++)
    {
        output[i] = (char *)malloc(sizeof(char)*c);
        for (size_t j = 0; j < c; j++)
        {
            output[i][j]=' ';
        }
        
    }
    return output;
}

Process ** readyProcesses(vector <string> p)
{
    Process** pr = (Process **)malloc(sizeof(Process)* p.size());
    for (size_t i = 0; i < p.size(); i++)
    {
        pr[i]=formulate_Process(p.at(i));
        pr[i]->processNumber = i;
    }
    return pr;
}

//________________________________FCFS______________________________________________________

void FCFS(vector <string> processes,const int instants, string status, string policyname)
{
    char** output = create_outputArray(processes.size(), instants);
    Process** pr = readyProcesses(processes);
    queue<Process *> q;
    Process* CPU=nullptr;
    int service;
    for (size_t i = 0; i < instants; i++)
    {
        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime)
            {
                if(pr[j]->ready == 0)
                {
                    q.push(pr[j]);
                    pr[j]->ready=1;
                    output[pr[j]->processNumber][i]='.';
                }
                else if (pr[j]->ready != -1)
                {
                   
                    output[pr[j]->processNumber][i]='.';
                }
            }
        }
        if(CPU==nullptr)
        {
            CPU = q.front();
            service = CPU->ServiceTime;
            q.pop();
        }
        if(CPU != nullptr)
        {
            service--;
            output[CPU->processNumber][i] = '*';
            if(service == 0)
            {
                CPU->ready = -1;
                CPU->FinishingTime = i+1;
                CPU = nullptr;
            }
        }   
    }
    string s = "trace";
    if (status.compare(s) == 0)
    {
        printTrace(instants,output,processes.size(),pr, policyname);
    }
    else
    {
        printStats(pr,processes.size(), policyname);
    }
 
        
}
//____________________________________________RR________________________________________

void RR(vector <string> processes,int quantum,const int instants, string status, string policyname)
{
    char** output = create_outputArray(processes.size(), instants);
    Process** pr = readyProcesses(processes);
    int processesServiceTimes[processes.size()];
    for (size_t i = 0; i < processes.size(); i++)
    {
        processesServiceTimes[i]= pr[i]->ServiceTime;
    }
    queue<Process *> q;
    Process* CPU=nullptr;
    Process* t = nullptr;
    int currentConsumption = 0;
    for (size_t i = 0; i < instants; i++)
    {
        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime)
            {
                if(pr[j]->ready == 0)
                {
                    q.push(pr[j]);
                    pr[j]->ready=1;
                    output[pr[j]->processNumber][i]='.';
                }
                else if (pr[j]->ready > 0)
                {
                   
                    output[pr[j]->processNumber][i]='.';
                }
            }
        }
        if (t != nullptr)
        {
            q.push(t);
            t = nullptr;
        }
        if (CPU == nullptr)
        {
                CPU = q.front();
                q.pop();
        }
        if (CPU != nullptr)
        {
            CPU->ServiceTime = CPU->ServiceTime - 1;
            currentConsumption++;
            output[CPU->processNumber][i] = '*';
            if (CPU->ServiceTime == 0)
            {
                CPU->ready = -1;
                CPU->FinishingTime = i+1;
                CPU = nullptr;
                currentConsumption = 0;
            }
            else if (currentConsumption >= quantum)
            {
                t = CPU;
                currentConsumption = 0;
                CPU = nullptr;
            }
        }
    }
    for (size_t i = 0; i < processes.size(); i++)
    {
        pr[i]->ServiceTime =processesServiceTimes[i];
    }
    string s = "trace";
    string quantum_str =to_string(quantum);
    policyname = policyname + quantum_str;
    if (status.compare(s) == 0)
    {
        printTrace(instants,output,processes.size(),pr, policyname);
    }
    else
    {
        printStats(pr,processes.size(), policyname);
    }
}

// __________________________________________SPN________________________________________________________
void SPN(vector <string> processes,const int instants, string status, string policyname)
{
    char** output = create_outputArray(processes.size(), instants);
    Process** pr = readyProcesses(processes);
    priority_queue<Process *, vector<Process *>, MyCompartor> q;
    Process* CPU=nullptr;
    int service;
    for (size_t i = 0; i < instants; i++)
    {
        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime)
            {
                if(pr[j]->ready == 0)
                {
                    q.push(pr[j]);
                    pr[j]->ready=1;
                    output[pr[j]->processNumber][i]='.';
                }
                else if (pr[j]->ready > 0)
                {
                   
                    output[pr[j]->processNumber][i]='.';
                }
            }
        }
        if (CPU == nullptr)
        {
                CPU = q.top();
                service = CPU->ServiceTime;
                q.pop();
        }
        if (CPU != nullptr)
        {
            service--;
            output[CPU->processNumber][i] = '*';
            if(service == 0)
            {
                CPU->ready = -1;
                CPU->FinishingTime = i+1;
                CPU = nullptr;
            }
        }
    }
    string s = "trace";
    if (status.compare(s) == 0)
    {
        printTrace(instants,output,processes.size(),pr, policyname);
    }
    else
    {
        printStats(pr,processes.size(), policyname);
    }
}

//_____________________________________________SRT______________________________________________
void SRT(vector <string> processes,const int instants, string status, string policyname)
{
    char** output = create_outputArray(processes.size(), instants);
    Process** pr = readyProcesses(processes);
    int processesServiceTimes[processes.size()];
    for (size_t i = 0; i < processes.size(); i++)
    {
        processesServiceTimes[i]= pr[i]->ServiceTime;
    }
    priority_queue<Process *, vector<Process *>, MyCompartor> q;
    Process* CPU=nullptr;
    for (size_t i = 0; i < instants; i++)
    {
        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime)
            {
                if(pr[j]->ready == 0)
                {
                    q.push(pr[j]);
                    pr[j]->ready=1;
                    output[pr[j]->processNumber][i]='.';
                }
                else if (pr[j]->ready > 0)
                {
                   
                    output[pr[j]->processNumber][i]='.';
                }
            }
        }
        if (CPU == nullptr)
        {
                CPU = q.top();
                q.pop();
        }
        if (CPU != nullptr)
        {
            CPU->ServiceTime = CPU->ServiceTime - 1;
            output[CPU->processNumber][i] = '*';
            if (CPU->ServiceTime == 0)
            {
                CPU->ready = -1;
                CPU->FinishingTime = i+1;
                CPU = nullptr;
            }
            else
            {
                q.push(CPU);
                CPU = nullptr;
            }
        }
    }
    for (size_t i = 0; i < processes.size(); i++)
    {
        pr[i]->ServiceTime =processesServiceTimes[i];
    }
    string s = "trace";
    if (status.compare(s) == 0)
    {
        printTrace(instants,output,processes.size(),pr, policyname);
    }
    else
    {
        printStats(pr,processes.size(), policyname);
    }
}

//______________________________________________HRRN__________________________________________________
void HRRN(vector <string> processes,const int instants, string status, string policyname)
{
    char** output = create_outputArray(processes.size(), instants);
    Process** pr = readyProcesses(processes);
    priority_queue<Process *, vector<Process *>, MyCompartor2> q;
    int processesServiceTimes[processes.size()];
    for (size_t i = 0; i < processes.size(); i++)
    {
        processesServiceTimes[i]= pr[i]->ServiceTime;
    }
    Process* CPU=nullptr;
    for (size_t i = 0; i < instants; i++)
    {
        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime)
            {
                if(pr[j]->ready == 0)
                {
                    q.push(pr[j]);
                    pr[j]->ready=1;
                    output[pr[j]->processNumber][i]='.';
                    pr[j]->WaitingTime++;
                }
                else if (pr[j]->ready > 0)
                {
                    pr[j]->WaitingTime++;
                    output[pr[j]->processNumber][i]='.';
                }
            }
        }
        if (CPU == nullptr)
        {
                CPU = q.top();
                CPU->WaitingTime--;
                q.pop();
        }
        if (CPU != nullptr)
        {
            CPU->ServiceTime = CPU->ServiceTime - 1;
            output[CPU->processNumber][i] = '*';
            if (CPU->ServiceTime == 0)
            {
                CPU->ready = -1;
                CPU->FinishingTime = i+1;
                CPU = nullptr;
            }
        }
    }
    for (size_t i = 0; i < processes.size(); i++)
    {
        pr[i]->ServiceTime =processesServiceTimes[i];
    }
    string s = "trace";
    if (status.compare(s) == 0)
    {
        printTrace(instants,output,processes.size(),pr, policyname);
    }
    else
    {
        printStats(pr,processes.size(), policyname);
    }
}

//_______________________________________________FB_____________________________________________________

int isArrayOfQueuesEmpty (vector <queue <Process *> > q,int start, int end)
{
    for (size_t i = start; i < end; i++)
    {
        if (!q.at(i).empty())
        {
            return 0;
        }
    }
    return 1;
}

void printqueue(vector <queue< Process *> > myq)
{
    vector <queue< Process *> > q = myq;
    for (size_t i = 0; i < q.size(); i++)
    {
        cout << "Queue Number  " << i << "  ";
        while (!q.at(i).empty())        
        {
            cout << q.at(i).front()->ProcessName << "  ";
            q.at(i).pop();
        }
        cout << endl;
    }
    
}

void FB(vector <string> processes,int quantum,const int instants, string status, string policyname)
{
    char** output = create_outputArray(processes.size(), instants);
    Process** pr = readyProcesses(processes);
    vector <queue< Process *> > q;
    for (size_t i = 0; i < processes.size(); i++)
    {
       queue <Process *> myq;
       q.push_back(myq);
    }
    int processesServiceTimes[processes.size()];
    for (size_t i = 0; i < processes.size(); i++)
    {
        processesServiceTimes[i]= pr[i]->ServiceTime;
    }
    Process* CPU=nullptr;
    Process* t=nullptr;
    int consumedQuantum = 0;
    int retriveQueueNum;
    char LastName;
    for (size_t i = 0; i < instants; i++)
    {
        
        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime)
            {
                if(pr[j]->ready == 0)
                {
                    q.at(0).push(pr[j]);
                    pr[j]->ready=1;
                    output[pr[j]->processNumber][i]='.';
                }
                else if (pr[j]->ready > 0)
                {
                   
                    output[pr[j]->processNumber][i]='.';
                }
            }
        }
        if (t != nullptr)
        {
            if (retriveQueueNum == q.size()-1 || isArrayOfQueuesEmpty(q,0,q.size())==1)
            {
                q.at(retriveQueueNum).push(t);
            }
            else
            {
            q.at(retriveQueueNum+1).push(t);
            }
            t = nullptr;
        }
        if (CPU == nullptr)
        {
            for (size_t k = 0; k < q.size(); k++)
            {
                if(!q.at(k).empty())
                {
                    Process * pt = q.at(k).front();
                    if (pt->ProcessName == LastName && isArrayOfQueuesEmpty(q,k+1,q.size())==0)
                    {
                        for (size_t h = k+1; h < q.size(); i++)
                        {
                            if (!q.at(h).empty())
                            {
                                CPU = q.at(h).front();
                                q.at(h).pop();
                                retriveQueueNum = h;
                                break;
                            }
                        }
                    }
                    else
                    {
                        CPU = q.at(k).front();
                        q.at(k).pop();
                        retriveQueueNum = k;
                    }
                break;
                }
               
            }
        }
        if (CPU != nullptr)
        {
            CPU->ServiceTime = CPU->ServiceTime - 1;
            consumedQuantum++;
            output[CPU->processNumber][i] = '*';
            
            if (CPU->ServiceTime == 0)
            {
                CPU->ready = -1;
                CPU->FinishingTime = i+1;
                CPU = nullptr;
                consumedQuantum = 0;
            }
            else if (consumedQuantum == pow(quantum,retriveQueueNum))
            {
                t = CPU;
                LastName = CPU->ProcessName;
                consumedQuantum = 0;
                CPU = nullptr;
            }
        }
    }
    for (size_t i = 0; i < processes.size(); i++)
    {
        pr[i]->ServiceTime =processesServiceTimes[i];
    }
    string s = "trace";
    if (status.compare(s) == 0)
    {
        printTrace(instants,output,processes.size(),pr, policyname);
    }
    else
    {
        printStats(pr,processes.size(), policyname);
    }
}
//_______________________________________________Aging____________________________________________________

void incrementPriorities(priority_queue<Process *, vector<Process *>, MyCompartor3> q)
{
    for (size_t i = 0; i < q.size(); i++)
    {
        Process* p = q.top();
        p->priority++;
        q.pop();
        q.push(p);
    }
    
}

void Aging(vector <string> processes,int quantum,const int instants, string status, string policyname)
{
    char** output = create_outputArray(processes.size(), instants);
    Process** pr = readyProcesses(processes);
    priority_queue<Process *, vector<Process *>, MyCompartor3> q;
    Process* CPU=nullptr;
    Process* t=nullptr;
    int consumedQuantum = 0;
    for (size_t i = 0; i < instants; i++)
    {
        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime)
            {
                if(pr[j]->ready == 0)
                {
                    q.push(pr[j]);
                    pr[j]->ready=1;
                    output[pr[j]->processNumber][i]='.';
                }
                else if (pr[j]->ready != -1)
                {
                   
                    output[pr[j]->processNumber][i]='.';
                }
            }
            
        }
        incrementPriorities(q);
        if (t != nullptr)
        {
            q.push(t);
            t = nullptr;
        }
        if (CPU == nullptr)
        {
            CPU = q.top();
            q.pop();
        }
        if (CPU != nullptr)
        {   
            consumedQuantum++;
            output[CPU->processNumber][i] = '*';
            if (consumedQuantum >= quantum)
            {
                CPU->priority = CPU->ServiceTime;
                t = CPU;
                consumedQuantum = 0;
                CPU = nullptr;
            }
        }

    }
    string s = "trace";
    if (status.compare(s) == 0)
    {
        printTrace(instants,output,processes.size(),pr, policyname);
    }
    else
    {
        printStats(pr,processes.size(), policyname);
    }
}
int countPolices(string p)
{
    int count = 0;
    for (size_t i = 0; i < p.size(); i++)
    {
        if (p.at(i)==',')
            count++;
    }
    return count + 1;
}
//____________________________________________________Main_______________________________________________
int main()
{
    string temp;
    vector <string> info;
    vector <string> processes;
    int line_number = 0;
    while (getline(cin, temp))
    {
        if (line_number < 4)
        {
            info.push_back(temp);
        }
        else
        {
            processes.push_back(temp);
        }
        line_number++;
    }
    string policyline = info.at(1);
    int NoOfPolicies = countPolices(policyline);
    string policies[NoOfPolicies];
    stringstream ss(policyline);
    int u=0;
    while(!ss.eof())
    {
        getline(ss,policies[u],',');
        u++;
    }
    for (size_t i = 0; i < NoOfPolicies; i++)
    {
    int policy = 0;
    int quantum = 0;
    if(policies[i].size() > 1)
    {
        stringstream ss2(policies[i]);
        int r = 0;
        string str[2];
        while (!ss2.eof())
        {
            getline(ss2,str[r],'-');
            r++;
        }
        policy = stoi(str[0]);
        quantum = stoi(str[1]);
    }
    else
    {
        policy = stoi(policies[i]);
    }
    switch (policy)
    {
        case 1:
            FCFS(processes,stoi(info.at(2)), info.at(0), "FCFS");
            break;
        case 2:
            RR(processes,quantum,stoi(info.at(2)), info.at(0), "RR-");
            break;
        case 3:
            SPN(processes, stoi(info.at(2)), info.at(0), "SPN");
            break;
        case 4:
            SRT(processes, stoi(info.at(2)), info.at(0), "SRT");
            break;
        case 5:
            HRRN(processes, stoi(info.at(2)), info.at(0), "HRRN");
            break;
        case 6:
            quantum = 1;
            FB(processes, quantum, stoi(info.at(2)), info.at(0), "FB-1");
            break;
        case 7:
            quantum = 2;
            FB(processes,quantum,stoi(info.at(2)), info.at(0), "FB-2i");
            break;
        case 8:
            Aging(processes,quantum,stoi(info.at(2)), info.at(0), "Aging");
    }
    }
}

