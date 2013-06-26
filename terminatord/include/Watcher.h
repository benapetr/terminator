#ifndef WATCHER_H
#define WATCHER_H


class Watcher
{
    public:
        static void Start();
        static bool Running;
        static unsigned long GetMemTotal();
        static unsigned long GetFree();
    protected:
    private:
        static void CheckSystemLimit();
        static void CheckUserLimit();
};

#endif // WATCHER_H
