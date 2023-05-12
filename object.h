#include <cstdlib>
#include "container.h"

class Object
{
private:
    inline static size_t counter = 0;

public:
    virtual std::string toString() const = 0;
    Object()
    {
        counter++;
    }

    Object(Object &other)
    {
        counter++;
    }

    virtual ~Object()
    {
        counter--;
    }

    static size_t get_counter()
    {
        return counter;
    }
};

class Task : virtual public Object
{
protected:
    bool executed;
    bool has_result_;

public:
    Task(bool has_result) : Object(), executed(false), has_result_(has_result) {}
    bool has_result()
    {
        return has_result_;
    }
    virtual void execute() = 0;
};

class Named : virtual public Object
{
protected:
    std::string name;

public:
    Named(const std::string &name) : Object(), name(name) {}

    std::string &get_name()
    {
        return name;
    }
};

class BinaryOperationTask : public Task, public Named
{
private:
    const double argument_1;
    const double argument_2;
    double result;

public:
    // default constructor - generates random task
    BinaryOperationTask() : Task(true),
                            Named("temp name"),
                            argument_1(rand() % 1000 / 10),
                            argument_2(1 + rand() % 1000 / 10)
    {
        size_t operation = rand() % 4;
        switch (operation)
        {
        case 0:
            name = "+";
            break;
        case 1:
            name = "-";
            break;
        case 2:
            name = "*";
            break;
        case 3:
            name = "/";
            break;
        default:
            break;
        }
    }

    BinaryOperationTask(
        double argument_1,
        double argument_2,
        std::string name) : Task(true), Named(name), argument_1(argument_1), argument_2(argument_2) {}

    virtual std::string toString() const
    {
        std::string info = "Operation: " + std::to_string(argument_1) + " " + name + " " + std::to_string(argument_2);
        if (executed)
        {
            info += "; Result: " + std::to_string(result);
        }

        return info;
    }

    virtual void execute()
    {
        if (name.size() != 1)
        {
            throw std::invalid_argument("Invalid operation");
        }

        switch (name[0]) // name for that operation contains only 1 symbol
        {
        case '+':
            result = argument_1 + argument_2;
            break;
        case '-':
            result = argument_1 - argument_2;
            break;
        case '*':
            result = argument_1 * argument_2;
            break;
        case '/':
            result = argument_1 / argument_2;
            break;
        default:
            throw std::invalid_argument("Invalid operation");
        }
        executed = true;
    }

    double get_result()
    {
        return result;
    }
};

class AddToContainerTask : public Task, public Named
{
private:
    Container<Task *> *tasks;
    Task *task;

public:
    AddToContainerTask(
        Container<Task *> *tasks,
        Task *task) : Task(false), Named("Adding to container"), tasks(tasks), task(task) {}

    virtual void execute()
    {
        tasks->push_back(task);
        executed = true;
    }

    virtual std::string toString() const
    {
        std::string info = name + ": {" + task->toString() + "}";
        if (executed)
        {
            info += "; Done.";
        }
        return info;
    }
};

class ObjectsNumber : public Task, public Named
{
private:
    Container<Task *> *objects;
    size_t result;

public:
    ObjectsNumber(Container<Task *> *objects) : Task(true), Named("Objects number"), objects(objects) {}

    virtual std::string toString() const
    {
        std::string info = "Objects number task";
        if (executed)
        {
            info += "; Result: " + std::to_string(result);
        }
        return info;
    }

    virtual void execute()
    {
        result = objects->size();
        executed = true;
    }

    size_t get_result()
    {
        return result;
    }
};

class TasksWithResultNumber : public Task, public Named
{
private:
    Container<Task *> *tasks;
    size_t result;

public:
    TasksWithResultNumber(Container<Task *> *tasks) : Task(true), Named("Tasks with result number"), tasks(tasks) {}

    virtual std::string toString() const
    {
        std::string info = "Tasks with result number tasks";
        if (executed)
        {
            info += "; Result: " + std::to_string(result);
        }
        return info;
    }

    virtual void execute()
    {
        result = 0;
        for (auto element = tasks->begin(); element != tasks->end(); ++element)
        {
            if (element.value()->has_result())
            {
                result++;
            }
        }
        executed = true;
    }

    size_t get_result()
    {
        return result;
    }
};

class ClearTask : public Task
{
private:
    Container<Task *> *objects;

public:
    ClearTask(Container<Task *> *objects) : Task(false), objects(objects) {}

    virtual ~ClearTask() {}

    virtual void execute()
    {
        objects->clear();
        executed = true;
    }

    virtual std::string toString() const
    {
        std::string info = "Clear task";
        if (executed)
        {
            info += "; Done.";
        }
        return info;
    }
};

class ObjectsNumberGlobal : public Task, public Named
{
private:
    size_t result;

public:
    ObjectsNumberGlobal() : Task(true), Named("Objects number global") {}

    virtual void execute()
    {
        result = Object::get_counter();
        executed = true;
    }

    virtual std::string toString() const
    {
        std::string info = "Objects number global task";
        if (executed)
        {
            info += "; Result: " + std::to_string(result);
        }
        return info;
    }

    size_t get_result()
    {
        return result;
    }
};
