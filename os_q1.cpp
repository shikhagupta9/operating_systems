
#include <iostream>
#include <list>
#include <queue>
#include <iterator>
#include <vector>

using namespace std;

typedef struct process
{
    int priority;
    int pid;
    int arrival;
    int total_burst;
    int tb; /*to retain original value of total burst*/
    int current_burst_cycle;
    int burst_executed;
    
    int current_io;
    int current_io_cycle;
    int rt;
    int tt;
    int io_cycle;
    int burst_cycle;
    int status;/* -1=not arrived ,0 =ready  */
    
} process;

/*----------------------------------------------------*/

process initialise(int pid,int at,int burst,int io_cycle,int burst_cycle );
void initialise_process_array(process p[]);

/*----------------------------------------------------*/


int main()
{
    process p[4];
    int alpha,beta;
    cout<<"enter alpha"<<endl;
    cin>>alpha;
    cout<<"enter beta"<<endl;
    cin>>beta;
    initialise_process_array(p);
    vector<process> ready,finished;
    queue<process> io;
    //vector<process> ::iterator vi;
    process *max_pro;
    vector<process>running;
    int max=-1;
    int cur=0;
    int index=-1;
    int context_switch=-1;
    while(finished.size()<4)
    {
        max_pro=NULL; //max_prio process
        max=-1;
        index=-1;
        if (!running.empty())
        {
            running[0].priority+=beta;
            running[0].burst_executed+=1;
            // running[0].total_burst--;
            running[0].current_burst_cycle+=1;
        }
        if (!io.empty())
        {
            io.front().current_io_cycle++;
        }
        for(auto it=ready.begin();it!=ready.end();it++)
        {
            
            it->priority+=alpha;
        }
        
        for (int i=0; i<4; i++)
        {
            if(p[i].arrival<=cur&&p[i].status==-1)
            {
                p[i].status=0;
                p[i].priority=0;
                ready.push_back(p[i]);
            }
        }
        
        if (!running.empty())
        {
            if (running[0].total_burst==running[0].burst_executed)
            {
                running[0].tt=cur-running[0].arrival;
                finished.push_back(running[0]);
                running.erase(running.begin());
            }
            else if(running[0].current_burst_cycle==running[0].burst_cycle)
            {
                running[0].current_burst_cycle=0;
                running[0].current_io_cycle=0;
                io.push(running[0]);
                running.erase(running.begin());
            }
        }
        if (!io.empty())
        {
            if (io.front().current_io_cycle==io.front().io_cycle)
            {
                io.front().priority=0;
                io.front().current_burst_cycle=0;
                ready.push_back(io.front());
                io.pop();
            }
        }
        
        
        if (!running.empty())
        {
            
            max_pro=&running[0];
            max=running[0].priority;
        }
        
        for(auto it=ready.begin();it!=ready.end();it++)
        {
            if (it->priority>max)
            {
                max=it->priority;
                max_pro=&*it;
                index=it-ready.begin();
            }
        }
        if (!running.empty())
        {
            if (max_pro!=&running[0])
            {
                
                ready.push_back(running[0]);
                running[0]=*max_pro;
                ready.erase(ready.begin()+index);
                if (running[0].rt==-1)
                {
                    running[0].rt=cur-running[0].arrival;
                }
                context_switch++;
            }
        }
        else
        {
            if(max_pro)
            {
                running.push_back(*max_pro);
                ready.erase(ready.begin()+index);
                if (running[0].rt==-1)
                {
                    running[0].rt=cur-running[0].arrival;
                }
                context_switch++;
            }
        }
        cout<<"process"<<running[0].pid<<" running"<<endl;
        cur++;
        
        
        
    }
    for (auto it=finished.begin(); it!=finished.end(); it++)
    {
        cout<<it->pid<<"\t"<<it->tt<<"\t"<<it->rt<<"\t"<<it->tt-it->tb<<endl;
        
    }
    cout<<"number of context switch is "<<context_switch<<endl;
    return 0;
}

/*----------------------------------------------------*/
process initialise(int pid,int at,int burst,int io_cycle,int burst_cycle )
{
    process p;
    p.arrival=at;
    p.pid=pid;
    p.total_burst=burst;
    p.tb=burst;
    p.burst_cycle=burst_cycle;
    p.burst_executed=0;
    p.io_cycle=io_cycle;
    p.priority=0;
    p.current_burst_cycle=0;
    p.current_io_cycle=0;
    p.current_io=0;
    p.tt=0;
    p.rt=-1;
    p.status=-1;
    return p;
}

/*----------------------------------------------------*/

void initialise_process_array(process p[])
{
    p[0]=initialise(0, 0, 18, 5, 5);
    p[1]=initialise(1, 3, 27, 6, 6);
    p[2]=initialise(2, 8, 44, 4, 6);
    p[3]=initialise(3, 12, 50, 3, 9);
}




