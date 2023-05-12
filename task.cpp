#include <cstdlib>
#include <vector>
#include <ctime>
#include "object.h"

// function for generating random BinaryOperationTask (0), ObjectsNumberGlobal (1),
// TasksWithResultNumber (2), ObjectsNumber (3)
void add_random_tasks(Container<Task *> *tasks)
{
    size_t tasks_number = 5 + rand() % 6;
    for (size_t i = 0; i < tasks_number; i++)
    {
        size_t type = rand() % 4;
        Task *task = nullptr;

        switch (type)
        {
        case 0:
            task = new BinaryOperationTask();
            break;
        case 1:
            task = new ObjectsNumberGlobal();
            break;
        case 2:
            task = new TasksWithResultNumber(tasks);
            break;
        case 3:
            task = new ObjectsNumber(tasks);
            break;
        default:
            break;
        }
        tasks->push_back(task);
    }
}

void task3()
{
    Container<Task *> tasks;
    Container<Task *> tasks_to_clear;
    Container<std::string> results;
    std::vector<Task *> to_delete;

    add_random_tasks(&tasks);          // add from 5 to 10 random tasks
    add_random_tasks(&tasks_to_clear); // add from 5 to 10 random tasks

    for (auto t = tasks_to_clear.begin(); t != tasks_to_clear.end(); ++t)
    {
        to_delete.push_back(t.value());
    }

    Task *task_1 = new BinaryOperationTask();
    Task *task_2 = new ObjectsNumberGlobal();
    Task *task_3 = new ClearTask(&tasks_to_clear);

    Task *task_4 = new AddToContainerTask(&tasks, task_1);
    Task *task_5 = new AddToContainerTask(&tasks, task_2);
    Task *task_6 = new AddToContainerTask(&tasks, task_3);

    tasks.push_back(task_4);
    tasks.push_back(task_5);
    tasks.push_back(task_6);
    std::cout << "Total objects number: " << std::to_string(Object::get_counter()) << std::endl;

    for (auto t = tasks.begin(); t != tasks.end(); ++t)
    {
        t.value()->execute();
        std::string result = t.value()->toString();
        results.push_back(result);
    }

    for (auto s = results.begin(); s != results.end(); ++s)
    {
        std::cout << s.value() << std::endl;
    }

    // delete all tasks
    for (size_t i = 0; i < to_delete.size(); ++i)
    {
        delete to_delete[i];
    }
    for (auto t = tasks.begin(); t != tasks.end(); ++t)
    {
        delete t.value();
    }

    tasks.clear();
    results.clear();
    to_delete.clear();
}

int main()
{
    srand(time(0));
    task3();
    std::cout << "Total objects number: " << std::to_string(Object::get_counter()) << std::endl;
    return 0;
}
