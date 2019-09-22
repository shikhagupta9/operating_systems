#include <iostream>
#include <vector>
#include <queue>
#include <iterator>
using namespace std;

typedef struct process
{
    int qno;
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
    int time_slice;
    int time_slice_exec;
    
} process;
/*----------------------------------------------------*/
process initialise(int qno,int pid,int at,int burst,int io_cycle,int burst_cycle,int ts );
/*----------------------------------------------------*/

int main(int argc, const char * argv[])
{
    
    vector<vector<process>> p(3);
    p[0].push_back(initialise(0,3, 12, 50, 3, 9,9));
    p[2].push_back(initialise(2,0, 0, 18, 5, 5,3));
    p[2].push_back(initialise(2,1, 3, 27, 6, 6,3));
    p[1].push_back(initialise(1,2, 8, 44, 4, 6,6));
    vector <process> finished,running;/*q2 highest priority*/
    vector<vector<process>> q(3);
    
    queue<process> io;
    
    int cur=0;
    int context_switch=-1;
    


    while(finished.size()<4)
    {
        if (!running.empty())
        {
            
            running[0].burst_executed+=1;
            // running[0].total_burst--;
            running[0].current_burst_cycle+=1;
            running[0].time_slice_exec++;
            /*if (running[0].burst_executed==running[0].total_burst)
            {
                running[0].tt=cur-running[0].arrival;
                finished.push_back(running[0]);
                running.erase(running.begin());
            }else if (running[0].current_burst_cycle==running[0].burst_cycle)
            {
                running[0].current_burst_cycle=0;
                running[0].current_io_cycle=0;
                running[0].time_slice_exec=0;
                io.push(running[0]);
                running.erase(running.begin());
            }else if (running[0].time_slice_exec==running[0].time_slice)
            {
                running[0].time_slice_exec=0;
                q[running[0].qno].push_back(running[0]);
                running.erase(running.begin());
            }*/
        }
        if (!io.empty())
        {
            io.front().current_io_cycle++;
        }
        for (auto it=p.begin();it!=p.end() ; it++)
        {
            for (auto iti=it->begin();iti!=it->end() ; iti++)
            {
                if (iti->arrival<=cur&&iti->status==-1)
                {
                    iti->status=0;
                    q[iti->qno].push_back(*iti);
                    
                }
            }
        }
        
        if (!running.empty())
        {
            if (running[0].burst_executed==running[0].total_burst)
            {
                running[0].tt=cur-running[0].arrival;
                finished.push_back(running[0]);
                running.erase(running.begin());
            }else if (running[0].current_burst_cycle==running[0].burst_cycle)
            {
                running[0].current_burst_cycle=0;
                running[0].current_io_cycle=0;
                running[0].time_slice_exec=0;
                io.push(running[0]);
                running.erase(running.begin());
            }else if (running[0].time_slice_exec==running[0].time_slice)
            {
                running[0].time_slice_exec=0;
                q[running[0].qno].push_back(running[0]);
                running.erase(running.begin());
            }
        }
        
        if (!io.empty())
        {
            if (io.front().current_io_cycle==io.front().io_cycle)
            {
                //io.front().priority=0;
                io.front().current_burst_cycle=0;
                io.front().time_slice_exec=0;
                q[io.front().qno].push_back(io.front());
                io.pop();
            }
        }
        if (running.empty())
        {
            if (!q[2].empty())
            {
                running.push_back(q[2][0]);
                q[2].erase(q[2].begin());
                context_switch++;
            }
            else if(!q[1].empty())
            {
                running.push_back(q[1][0]);
                q[1].erase(q[1].begin());
                context_switch++;
            }
            else if(!q[0].empty())
            {
                running.push_back(q[1][0]);
                q[0].erase(q[0].begin());
                context_switch++;
            }
            if (!running.empty())
            {
                if (running[0].rt==-1)
                {
                    running[0].rt=cur-running[0].arrival;
                }
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
process initialise(int qno,int pid,int at,int burst,int io_cycle,int burst_cycle,int ts )
{
    process p;
    p.qno=qno;
    p.arrival=at;
    p.pid=pid;
    p.total_burst=burst;
    p.tb=burst;
    p.burst_cycle=burst_cycle;
    p.burst_executed=0;
    p.io_cycle=io_cycle;
    //p.priority=0;
    p.current_burst_cycle=0;
    p.current_io_cycle=0;
    p.current_io=0;
    p.tt=0;
    p.rt=-1;
    p.status=-1;
    p.time_slice=ts;
    p.time_slice_exec=0;
    return p;
}
