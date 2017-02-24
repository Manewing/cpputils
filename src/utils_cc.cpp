#include "utils_cc.h"

#ifdef UTILS_CC_THREAD_SAFE
#include <mutex>
namespace {
  std::mutex __cc_if_mtx;
}
#define MTX_LOCK    __cc_if_mtx.lock()
#define MTX_UNLOCK  __cc_if_mtx.unlock()
#else
#define MTX_LOCK
#define MTX_UNLOCK
#endif

namespace utils {

  __cc_param::__cc_param() {
    /* nothing todo */
  }

  __cc_param::~__cc_param() {
    MTX_LOCK;

    // get interface which is mapped to this parameter
    cc_if* __if = cc_if::ctx[this];
    cc_if::ctx.erase(this);

    MTX_UNLOCK;

    // call cleanup function
    __if->cc_up();
  }

  cc_if::context_t  cc_if::ctx;

  cc_if::cc_if(__cc_param const& __cc_p) {
    MTX_LOCK;

    // map this interface to the parameter
    cc_if::ctx[&__cc_p] = this;

    MTX_UNLOCK;
  }

}; // namespace utils
