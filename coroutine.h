#define _XOPEN_SOURCE
#include <stdlib.h>
#include <ucontext.h>
#include <memory>

class Coroutine {
  public:
    typedef void(entry_t)(void*);

  private:
    class Thread& thread;
    size_t id;
    ucontext_t context;
    std::auto_ptr<char> stack;

    static void trampoline(Coroutine& that, entry_t& entry, void* arg);

  public:
    /**
     * Returns the currently-running fiber.
     */
    static Coroutine& current();

    /**
     * Is Coroutine-local storage via pthreads enabled? The Coroutine library should work fine
     * without this, but programs that are not aware of coroutines may panic if they make
     * assumptions about the stack. In order to enable this you must LD_PRELOAD (or equivalent)
     * this library.
     */
    static const bool is_local_storage_enabled();

    /**
     * Constructor for currently running "fiber". This is really just original thread, but we
     * need a way to get back into the main thread after yielding to a fiber. Basically this
     * shouldn't be called from anywhere.
     */
    Coroutine(Thread& t, size_t id);

    /**
     * This constructor will actually create a new fiber context. Execution does not begin
     * until you call run() for the first time.
     */
    Coroutine(Thread& t, size_t id, entry_t& entry, void* arg);

    /**
     * Don't delete Coroutines, they will delete themselves.
     * TODO: Actually they don't!
     */
    ~Coroutine();

    /**
     * Start or resume execution in this fiber. Note there is no explicit yield() function,
     * you must manually run another fiber.
     */
    void run();

    /**
     * Create a new fiber. This just calls back into Thread because Coroutine is the only public
     * class in this library.
     */
    Coroutine& new_fiber(entry_t* entry, void* arg = NULL);

    /**
     * Returns address of the lowest usable byte in this Coroutine's stack.
     */
    void* bottom() const;

    bool operator==(const Coroutine& that) const;
    bool operator==(const Coroutine* that) const;

    size_t getid() const {
      return id;
    }
};