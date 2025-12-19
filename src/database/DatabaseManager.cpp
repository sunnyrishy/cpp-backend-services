#include "database/DatabaseManager.h"

// Initialize static member
namespace database {
    DatabaseManager* DatabaseManager::instance_ = nullptr;
}