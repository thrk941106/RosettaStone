// Copyright (c) 2018 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef HEARTHSTONEPP_DAMAGE_TASK_HPP
#define HEARTHSTONEPP_DAMAGE_TASK_HPP

#include <hspp/Tasks/Tasks.hpp>

namespace Hearthstonepp::BasicTasks
{
//!
//! \brief DamageTask class.
//!
//! This class represents the task for taking damage.
//!
class DamageTask : public ITask
{
 public:
    //! Constructs task with given \p character and \p damage.
    //! \param character A pointer to character to take damage.
    //! \param damage A value indicating how much to take.
    DamageTask(Character* character, size_t damage);

    //! Returns task ID.
    //! \return Task ID.
    TaskID GetTaskID() const override;

 private:
    //! Processes task logic internally and returns meta data.
    //! \param player The player to run task.
    //! \return The result of task processing.
    MetaData Impl(Player& player) override;

    Character* m_character = nullptr;
    size_t m_damage = 0;
};
}  // namespace Hearthstonepp::BasicTasks

#endif  // HEARTHSTONEPP_DAMAGE_TASK_HPP