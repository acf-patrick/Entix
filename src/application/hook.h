/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Define hook for application setup
 */

#pragma once

#include "application.h"

namespace entix {
namespace core {

class ApplicationHook {
   protected:
    virtual ~ApplicationHook() = default;

   public:
    virtual void startup() { /* override */ }
    virtual void cleanup() { /* override */ }
};

}  // namespace application
}  // namespace entix