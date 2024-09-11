/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Define hook for application setup
 */

#pragma once

#include "application.h"

class ApplicationHook {
   protected:
    Application& application;
    ApplicationHook() : application(Application::Get()) {}
    virtual ~ApplicationHook() = default;

   public:
    virtual void startup() { /* override */
    }
    virtual void cleanup() { /* override */
    }
};