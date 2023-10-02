#include <iostream>
#include <string>

#include "jobsystem.h"
#include "compilejob.h"

using namespace std;

int main()
{
    // Create job system object
    JobSystem *js = JobSystem::CreateOrGet();

    // Create a vector with different commands
    vector<std::string> commands = {"make -s -C Code project1",
                                    "make -s -C Code project2",
                                    "make -s -C Code project3",
                                    "make -s -C Code project4"};

    // Create the maximum thread amount supported by the system
    cout << "Creating Worker Threads" << endl;
    for (int i = 0; i < thread::hardware_concurrency() - 1; i++)
    {
        js->CreateWorkerThread(("Thread" + std::to_string(i)).c_str(), 0xFFFFFFFF);
    }

    // Create a different job for each make command (makefile needed)
    cout << "Create Jobs" << endl;
    vector<Job *> jobs;
    for (int i = 0; i < commands.size(); i++)
    {
        CompileJob *cjb = new CompileJob(0xFFFFFFFF, i, commands[i]);
        jobs.push_back(cjb);
    }

    // Queue the jobs
    cout << "Queuing Jobs" << endl;
    vector<Job *>::iterator it = jobs.begin();
    for (; it != jobs.end(); it++)
    {
        js->QueueJob(*it);
    }

    // Finish all jobs
    js->FinishCompletedJobs();

    // Check all job statuses
    std::cout << "Job Status" << std::endl;
    for (int i = 0; i < jobs.size(); i++)
    {
        std::cout << "Job " << i << " Status: " << (int)js->GetJobStatus(i) << std::endl;
    }

    // Destroy Job System
    js->Destroy();

    return 0;
}