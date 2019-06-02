﻿// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Utils/CardSetUtils.hpp>

#include <Rosetta/Actions/Draw.hpp>
#include <Rosetta/Cards/Cards.hpp>

using namespace RosettaStone;
using namespace PlayerTasks;
using namespace SimpleTasks;

// ---------------------------------------- MINION - HUNTER
// [EX1_543] King Krush - COST:9 [ATK:8/HP:8]
// - Race: Beast, Faction: Neutral, Set: Expert1, Rarity: Legendary
// --------------------------------------------------------
// Text: <b>Charge</b>
// --------------------------------------------------------
// GameTag:
// - ELITE = 1
// - CHARGE = 1
// --------------------------------------------------------
TEST(HunterExpert1Test, EX1_543_KingKrush)
{
    // Do nothing
}

// ------------------------------------------- SPELL - MAGE
// [CS2_028] Blizzard - COST:6
// - Faction: Neutral, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: Deal $2 damage to all enemy minions and <b>Freeze</b> them.
// --------------------------------------------------------
// GameTag:
// - FREEZE = 1
// --------------------------------------------------------
TEST(MageExpert1Test, CS2_028_Blizzard)
{
    GameConfig config;
    config.player1Class = CardClass::MAGE;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Blizzard"));
    const auto card2 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Boulderfist Ogre"));
    const auto card3 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card2));
    game.Process(opPlayer, PlayCardTask::Minion(card3));
    EXPECT_EQ(opField.GetCount(), 2);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    EXPECT_EQ(opField.GetCount(), 1);
    EXPECT_EQ(opField[0]->GetHealth(), 5);
    EXPECT_EQ(opField[0]->GetGameTag(GameTag::FROZEN), 1);
    EXPECT_EQ(opPlayer.GetHero()->GetHealth(), 30);
    EXPECT_EQ(opPlayer.GetHero()->GetGameTag(GameTag::FROZEN), 0);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, AttackTask(card2, curPlayer.GetHero()));
    EXPECT_EQ(opField[0]->GetGameTag(GameTag::FROZEN), 1);
    EXPECT_EQ(curPlayer.GetHero()->GetHealth(), 30);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(opField[0]->GetGameTag(GameTag::FROZEN), 0);
}

// ------------------------------------------- SPELL - MAGE
// [EX1_279] Pyroblast - COST:10
// - Faction: Neutral, Set: Expert1, Rarity: Epic
// --------------------------------------------------------
// Text: Deal $10 damage.
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// --------------------------------------------------------
TEST(MageExpert1Test, EX1_279_Pyroblast)
{
    GameConfig config;
    config.player1Class = CardClass::MAGE;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Pyroblast"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Pyroblast"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Pyroblast"));
    const auto card4 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Pyroblast"));
    const auto card5 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));
    const auto card6 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));

    game.Process(curPlayer, PlayCardTask::Minion(card5));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card6));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card1, card6));
    EXPECT_EQ(opPlayer.GetFieldZone().GetCount(), 0);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer,
                 PlayCardTask::SpellTarget(card2, opPlayer.GetHero()));
    EXPECT_EQ(opPlayer.GetHero()->GetHealth(), 20);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card3, card5));
    EXPECT_EQ(curPlayer.GetFieldZone().GetCount(), 0);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer,
                 PlayCardTask::SpellTarget(card4, curPlayer.GetHero()));
    EXPECT_EQ(curPlayer.GetHero()->GetHealth(), 20);
}

// ------------------------------------------- SPELL - MAGE
// [EX1_287] Counterspell - COST:3
// - Faction: Neutral, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Secret:</b> When your opponent casts a spell, <b>Counter</b> it.
// --------------------------------------------------------
// GameTag:
// - SECRET = 1
// --------------------------------------------------------
// RefTag:
// - COUNTER = 1
// --------------------------------------------------------
TEST(MageExpert1Test, EX1_287_Counterspell)
{
    GameConfig config;
    config.player1Class = CardClass::MAGE;
    config.player2Class = CardClass::SHAMAN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Counterspell"));
    const auto card2 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Lightning Bolt"));

    game.Process(curPlayer, PlayCardTask::Spell(card1));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer,
                 PlayCardTask::SpellTarget(card2, curPlayer.GetHero()));
    EXPECT_EQ(curPlayer.GetHero()->GetHealth(), 30);
    EXPECT_EQ(opPlayer.GetRemainingMana(), 9);
    EXPECT_EQ(opPlayer.GetOverloadOwed(), 1);
}

// ---------------------------------------- SPELL - WARRIOR
// [CS2_104] Rampage - COST:2
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: Give a damaged minion +3/+3.
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// - REQ_MINION_TARGET = 0
// - REQ_DAMAGED_TARGET = 0
// --------------------------------------------------------
TEST(WarriorExpert1Test, CS2_104_Rampage)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Boulderfist Ogre"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Rampage"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    EXPECT_EQ(curField[0]->GetAttack(), 6);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card2, card1));
    EXPECT_EQ(curField[0]->GetAttack(), 6);

    curField[0]->SetDamage(1);
    game.Process(curPlayer, PlayCardTask::SpellTarget(card2, card1));
    EXPECT_EQ(curField[0]->GetAttack(), 9);
    EXPECT_EQ(curField[0]->GetHealth(), 9);
}

// ----------------------------------------- SPELL - WARRIOR
// [EX1_607] Inner Rage - COST:0
// - Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: Deal $1 damage to a minion and give it +2 Attack.
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// - REQ_MINION_TARGET = 0
// --------------------------------------------------------
TEST(WarriorExpert1Test, EX1_607_InnerRage)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Boulderfist Ogre"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Inner Rage"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    EXPECT_EQ(curField[0]->GetAttack(), 6);
    EXPECT_EQ(curField[0]->GetHealth(), 7);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card2, card1));
    EXPECT_EQ(curField[0]->GetAttack(), 8);
    EXPECT_EQ(curField[0]->GetHealth(), 6);
}

// ----------------------------------------- SPELL - PRIEST
// [CS1_129] Inner Fire - COST:1
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: Change a minion's Attack to be equal to its Health.
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// - REQ_MINION_TARGET = 0
// --------------------------------------------------------
TEST(PriestExpert1Test, CS1_129_InnerFire)
{
    GameConfig config;
    config.player1Class = CardClass::PRIEST;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Inner Fire"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Divine Spirit"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Inner Fire"));
    const auto card4 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Inner Fire"));
    const auto card5 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Northshire Cleric"));

    game.Process(curPlayer, PlayCardTask::Minion(card5));
    EXPECT_EQ(curField.GetCount(), 1);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card1, curField[0]));
    EXPECT_EQ(curField[0]->GetHealth(), curField[0]->GetAttack());

    game.Process(curPlayer, PlayCardTask::SpellTarget(card2, curField[0]));
    EXPECT_EQ(curField[0]->GetHealth(), 6);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card3, curField[0]));
    EXPECT_EQ(curField[0]->GetHealth(), curField[0]->GetAttack());

    curField[0]->SetDamage(1);
    EXPECT_EQ(curField[0]->GetHealth(), 5);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card4, curField[0]));
    EXPECT_EQ(curField[0]->GetHealth(), curField[0]->GetAttack());
}

// ----------------------------------------- SPELL - PRIEST
// [EX1_621] Circle of Healing - COST:0
// - Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: Restore #4 Health to ALL minions.
// --------------------------------------------------------
TEST(PriestExpert1Test, EX1_621_CircleOfHealing)
{
    GameConfig config;
    config.player1Class = CardClass::PRIEST;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();
    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Bloodmage Thalnos"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Circle of Healing"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Boulderfist Ogre"));
    const auto card4 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Boulderfist Ogre"));
    const auto card5 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    Task::Run(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    Task::Run(opPlayer, PlayCardTask::Minion(card4));
    Task::Run(opPlayer, PlayCardTask::Minion(card5));
    EXPECT_EQ(opField.GetCount(), 2);

    Task::Run(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    Task::Run(curPlayer, PlayCardTask::Minion(card1));
    Task::Run(curPlayer, PlayCardTask::Minion(card3));

    Task::Run(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    Task::Run(opPlayer, AttackTask(card5, card3));
    Task::Run(opPlayer, AttackTask(card4, card1));
    EXPECT_EQ(curField.GetCount(), 1);
    EXPECT_EQ(curField[0]->GetHealth(), 4);
    EXPECT_EQ(opField[0]->GetHealth(), 6);

    Task::Run(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    Task::Run(curPlayer, PlayCardTask::Spell(card2));
    EXPECT_EQ(curField[0]->GetHealth(), 7);
    EXPECT_EQ(opField[0]->GetHealth(), 7);
}

// ---------------------------------------- MINION - PRIEST
// [EX1_623] Temple Enforcer - COST:6 [ATK:6/HP:6]
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Battlecry:</b> Give a friendly minion +3 Health.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// --------------------------------------------------------
// PlayReq:
// - REQ_FRIENDLY_TARGET = 0
// - REQ_MINION_TARGET = 0
// - REQ_TARGET_IF_AVAILABLE = 0
// --------------------------------------------------------
TEST(PriestExpert1Test, EX1_623_TempleEnforcer)
{
    GameConfig config;
    config.player1Class = CardClass::PRIEST;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Temple Enforcer"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    Task::Run(curPlayer, PlayCardTask::Minion(card2));
    EXPECT_EQ(curField[0]->GetAttack(), 3);
    EXPECT_EQ(curField[0]->GetHealth(), 1);

    Task::Run(curPlayer, PlayCardTask::MinionTarget(card1, card2));
    EXPECT_EQ(curField[0]->GetAttack(), 3);
    EXPECT_EQ(curField[0]->GetHealth(), 4);
}

// ----------------------------------------- SPELL - PRIEST
// [EX1_624] Holy Fire - COST:6
// - Faction: Priest, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: Deal $5 damage. Restore #5 Health to your hero.
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// --------------------------------------------------------
TEST(PriestExpert1Test, EX1_624_HolyFire)
{
    GameConfig config;
    config.player1Class = CardClass::PRIEST;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);
    curPlayer.GetHero()->SetDamage(8);

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Holy Fire"));

    game.Process(curPlayer,
                 PlayCardTask::SpellTarget(card1, opPlayer.GetHero()));
    EXPECT_EQ(curPlayer.GetHero()->GetHealth(), 27);
    EXPECT_EQ(opPlayer.GetHero()->GetHealth(), 25);
}

// ------------------------------------------ SPELL - ROGUE
// [CS2_073] Cold Blood - COST:1
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: Give a minion +2 Attack. <b>Combo:</b> +4 Attack instead.
// --------------------------------------------------------
// GameTag:
// - COMBO = 1
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// - REQ_MINION_TARGET = 0
// --------------------------------------------------------
TEST(RogueExpert1Test, CS2_073_ColdBlood)
{
    GameConfig config;
    config.player1Class = CardClass::ROGUE;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Cold Blood"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Cold Blood"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));
    const auto card4 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    game.Process(curPlayer, PlayCardTask::Minion(card3));
    game.Process(curPlayer, PlayCardTask::Minion(card4));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card1, card3));
    EXPECT_EQ(curField[0]->GetAttack(), 5);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card2, card4));
    EXPECT_EQ(curField[1]->GetAttack(), 7);
}

// ------------------------------------------ SPELL - ROGUE
// [EX1_124] Eviscerate - COST:2
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: Deal $2 damage. <b>Combo:</b> Deal $4 damage instead. @spelldmg
// --------------------------------------------------------
// GameTag:
// - COMBO = 1
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// --------------------------------------------------------
TEST(RogueExpert1Test, EX1_124_Eviscerate)
{
    GameConfig config;
    config.player1Class = CardClass::ROGUE;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Eviscerate"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Eviscerate"));

    game.Process(curPlayer,
                 PlayCardTask::SpellTarget(card1, opPlayer.GetHero()));
    EXPECT_EQ(opPlayer.GetHero()->GetHealth(), 28);

    game.Process(curPlayer,
                 PlayCardTask::SpellTarget(card2, opPlayer.GetHero()));
    EXPECT_EQ(opPlayer.GetHero()->GetHealth(), 24);
}

// ----------------------------------------- MINION - ROGUE
// [EX1_131] Defias Ringleader - COST:2 [ATK:2/HP:2]
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Combo:</b> Summon a 2/1 Defias Bandit.
// --------------------------------------------------------
// GameTag:
// - COMBO = 1
// --------------------------------------------------------
TEST(RogueExpert1Test, EX1_131_DefiasRingleader)
{
    GameConfig config;
    config.player1Class = CardClass::ROGUE;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Defias Ringleader"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Defias Ringleader"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    EXPECT_EQ(curField.GetCount(), 1);
    EXPECT_EQ(curField[0]->GetAttack(), 2);
    EXPECT_EQ(curField[0]->GetHealth(), 2);

    game.Process(curPlayer, PlayCardTask::Minion(card2));
    EXPECT_EQ(curField.GetCount(), 3);
    EXPECT_EQ(curField[1]->GetAttack(), 2);
    EXPECT_EQ(curField[1]->GetHealth(), 2);
    EXPECT_EQ(curField[2]->GetAttack(), 2);
    EXPECT_EQ(curField[2]->GetHealth(), 1);
}

// ----------------------------------------- MINION - ROGUE
// [EX1_134] SI:7 Agent - COST:3 [ATK:3/HP:3]
// - Faction: Neutral, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Combo:</b> Deal 2 damage.
// --------------------------------------------------------
// GameTag:
// - COMBO = 1
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_FOR_COMBO = 0
// --------------------------------------------------------
TEST(RogueExpert1Test, EX1_134_SI7Agent)
{
    GameConfig config;
    config.player1Class = CardClass::ROGUE;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("SI:7 Agent"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("SI:7 Agent"));

    game.Process(curPlayer,
                 PlayCardTask::SpellTarget(card1, opPlayer.GetHero()));
    EXPECT_EQ(opPlayer.GetHero()->GetHealth(), 30);

    game.Process(curPlayer,
                 PlayCardTask::SpellTarget(card2, opPlayer.GetHero()));
    EXPECT_EQ(opPlayer.GetHero()->GetHealth(), 28);
}

// ----------------------------------------- MINION - ROGUE
// [EX1_522] Patient Assassin - COST:2 [ATK:1/HP:1]
// - Faction: Neutral, Set: Expert1, Rarity: Epic
// --------------------------------------------------------
// Text: <b>Stealth</b>
//        <b>Poisonous</b>
// --------------------------------------------------------
// GameTag:
// - STEALTH = 1
// - POISONOUS = 1
// --------------------------------------------------------
TEST(RogueExpert1Test, EX1_522_PatientAssassin)
{
    // Do nothing
}

// ----------------------------------------- MINION - ROGUE
// [NEW1_005] Kidnapper - COST:6 [ATK:5/HP:3]
// - Faction: Neutral, Set: Expert1, Rarity: Epic
// --------------------------------------------------------
// Text: <b>Combo:</b> Return a minion to its owner's hand.
// --------------------------------------------------------
// GameTag:
// - COMBO = 1
// --------------------------------------------------------
// PlayReq:
// - REQ_MINION_TARGET = 0
// - REQ_TARGET_FOR_COMBO = 0
// --------------------------------------------------------
TEST(RogueExpert1Test, NEW1_005_Kidnapper)
{
    GameConfig config;
    config.player1Class = CardClass::ROGUE;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& opField = opPlayer.GetFieldZone();
    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Kidnapper"));
    const auto card4 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Kidnapper"));
    const auto card5 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    EXPECT_EQ(curField.GetCount(), 1);
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 7);
    EXPECT_EQ(curField[0]->GetAttack(), 3);
    EXPECT_EQ(curField[0]->GetHealth(), 2);

    game.Process(curPlayer, PlayCardTask::MinionTarget(card3, card1));
    EXPECT_EQ(curField.GetCount(), 1);
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 7);
    EXPECT_EQ(curField[0]->GetAttack(), 5);
    EXPECT_EQ(curField[0]->GetHealth(), 3);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card5));
    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer, PlayCardTask::Minion(card2));
    EXPECT_EQ(opField.GetCount(), 1);
    EXPECT_EQ(opPlayer.GetHandZone().GetCount(), 6);

    game.Process(curPlayer, PlayCardTask::MinionTarget(card4, card5));
    EXPECT_EQ(opField.GetCount(), 0);
    EXPECT_EQ(opPlayer.GetHandZone().GetCount(), 7);
}

// ----------------------------------------- SPELL - SHAMAN
// [CS2_038] Ancestral Spirit - COST:2
// - Faction: Neutral, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: Give a minion "<b>Deathrattle:</b> Resummon this minion."
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// - REQ_MINION_TARGET = 0
// --------------------------------------------------------
// RefTag:
// - DEATHRATTLE = 1
// --------------------------------------------------------
TEST(ShamanExpert1Test, CS2_038_AncestralSpirit)
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Ancestral Spirit"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));
    const auto card3 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    game.Process(curPlayer, PlayCardTask::Minion(card2));
    game.Process(curPlayer, PlayCardTask::SpellTarget(card1, card2));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card3));
    game.Process(opPlayer, AttackTask(card3, card1));
    EXPECT_EQ(curField.GetCount(), 1);
    EXPECT_EQ(curField[0]->GetAttack(), 3);
    EXPECT_EQ(curField[0]->GetHealth(), 2);
}

// ----------------------------------------- SPELL - SHAMAN
// [CS2_053] Far Sight - COST:3
// - Faction: Neutral, Set: Expert1, Rarity: Epic
// --------------------------------------------------------
// Text: Draw a card. That card costs (3) less.
// --------------------------------------------------------
TEST(ShamanExpert1Test, CS2_053_FarSight)
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curHand = curPlayer.GetHandZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Far Sight"));

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    EXPECT_EQ(curHand.GetCount(), 5);

    Entity* drawCard = curHand[curHand.GetCount() - 1];
    int cost = drawCard->card.gameTags[GameTag::COST] - 3;
    EXPECT_EQ(cost < 0 ? 0 : cost, drawCard->GetCost());
}

// ----------------------------------------- SPELL - SHAMAN
// [EX1_238] Lightning Bolt - COST:1
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: Deal $3 damage. <b>Overload:</b> (1)
// --------------------------------------------------------
// GameTag:
// - OVERLOAD = 1
// - OVERLOAD_OWED = 1
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// --------------------------------------------------------
TEST(ShamanExpert1Test, EX1_238_LightningBolt)
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Lightning Bolt"));

    game.Process(curPlayer,
                 PlayCardTask::SpellTarget(card1, opPlayer.GetHero()));
    EXPECT_EQ(opPlayer.GetHero()->GetHealth(), 27);
    EXPECT_EQ(curPlayer.GetRemainingMana(), 9);
    EXPECT_EQ(curPlayer.GetOverloadOwed(), 1);
    EXPECT_EQ(curPlayer.GetOverloadLocked(), 0);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(curPlayer.GetRemainingMana(), 9);
    EXPECT_EQ(curPlayer.GetOverloadOwed(), 0);
    EXPECT_EQ(curPlayer.GetOverloadLocked(), 1);
}

// ----------------------------------------- SPELL - SHAMAN
// [EX1_241] Lava Burst - COST:3
// - Faction: Neutral, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: Deal $5 damage. <b>Overload:</b> (2)
// --------------------------------------------------------
// GameTag:
// - OVERLOAD = 2
// - OVERLOAD_OWED = 2
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// --------------------------------------------------------
TEST(ShamanExpert1Test, EX1_241_LavaBurst)
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Lava Burst"));

    game.Process(curPlayer,
                 PlayCardTask::SpellTarget(card1, opPlayer.GetHero()));
    EXPECT_EQ(opPlayer.GetHero()->GetHealth(), 25);
    EXPECT_EQ(curPlayer.GetRemainingMana(), 7);
    EXPECT_EQ(curPlayer.GetOverloadOwed(), 2);
    EXPECT_EQ(curPlayer.GetOverloadLocked(), 0);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(curPlayer.GetRemainingMana(), 8);
    EXPECT_EQ(curPlayer.GetOverloadOwed(), 0);
    EXPECT_EQ(curPlayer.GetOverloadLocked(), 2);
}

// ----------------------------------------- SPELL - SHAMAN
// [EX1_248] Feral Spirit - COST:3
// - Faction: Neutral, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: Summon two 2/3 Spirit Wolves with <b>Taunt</b>. <b>Overload:</b> (2)
// --------------------------------------------------------
// GameTag:
// - OVERLOAD = 2
// - OVERLOAD_OWED = 2
// --------------------------------------------------------
// PlayReq:
// - REQ_NUM_MINION_SLOTS = 1
// --------------------------------------------------------
TEST(ShamanExpert1Test, EX1_248_FeralSpirit)
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Feral Spirit"));

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    EXPECT_EQ(curField.GetCount(), 2);
    EXPECT_EQ(curField[0]->GetGameTag(GameTag::TAUNT), 1);
    EXPECT_EQ(curField[0]->GetAttack(), 2);
    EXPECT_EQ(curField[0]->GetHealth(), 3);
    EXPECT_EQ(curField[1]->GetGameTag(GameTag::TAUNT), 1);
    EXPECT_EQ(curField[1]->GetAttack(), 2);
    EXPECT_EQ(curField[1]->GetHealth(), 3);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(curPlayer.GetRemainingMana(), 8);
    EXPECT_EQ(curPlayer.GetOverloadOwed(), 0);
    EXPECT_EQ(curPlayer.GetOverloadLocked(), 2);
}

// ----------------------------------------- SPELL - SHAMAN
// [EX1_251] Forked Lightning - COST:1
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: Deal $2 damage to 2 random enemy minions. <b>Overload:</b> (2)
// --------------------------------------------------------
// GameTag:
// - OVERLOAD = 2
// - OVERLOAD_OWED = 2
// --------------------------------------------------------
// PlayReq:
// - REQ_MINIMUM_ENEMY_MINIONS = 2
// --------------------------------------------------------
TEST(ShamanExpert1Test, EX1_251_ForkedLightning)
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Forked Lightning"));
    const auto card2 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Boulderfist Ogre"));
    const auto card3 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Boulderfist Ogre"));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card2));
    EXPECT_EQ(opField[0]->GetHealth(), 7);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 6);
    EXPECT_EQ(opField[0]->GetHealth(), 7);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card3));
    EXPECT_EQ(opField[1]->GetHealth(), 7);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 6);
    EXPECT_EQ(opField[0]->GetHealth(), 5);
    EXPECT_EQ(opField[1]->GetHealth(), 5);
    EXPECT_EQ(curPlayer.GetRemainingMana(), 9);
    EXPECT_EQ(curPlayer.GetOverloadOwed(), 2);
    EXPECT_EQ(curPlayer.GetOverloadLocked(), 0);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(curPlayer.GetRemainingMana(), 8);
    EXPECT_EQ(curPlayer.GetOverloadOwed(), 0);
    EXPECT_EQ(curPlayer.GetOverloadLocked(), 2);
}

// ---------------------------------------- MINION - SHAMAN
// [NEW1_010] Al'Akir the Windlord - COST:8 [ATK:3/HP:5]
// - Race: Elemental, Set: Expert1, Rarity: Legendary
// --------------------------------------------------------
// Text: <b>Charge, Divine Shield, Taunt, Windfury</b>
// --------------------------------------------------------
// GameTag:
// - ELITE = 1
// - WINDFURY = 1
// - TAUNT = 1
// - DIVINE_SHIELD = 1
// - CHARGE = 1
// --------------------------------------------------------
TEST(ShamanExpert1Test, NEW1_010_AlAkirTheWindlord)
{
    // Do nothing
}

// --------------------------------------- MINION - WARLOCK
// [CS2_059] Blood Imp - COST:1 [ATK:0/HP:1]
// - Race: Demon, Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Stealth</b>. At the end of your turn,
//       give another random friendly minion +1 Health.
// --------------------------------------------------------
// GameTag:
// - STEALTH = 1
// --------------------------------------------------------
TEST(WarlockExpert1Test, CS2_059_BloodImp)
{
    GameConfig config;
    config.player1Class = CardClass::WARLOCK;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Blood Imp"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    game.Process(curPlayer, PlayCardTask::Minion(card2));
    game.Process(curPlayer, PlayCardTask::Minion(card3));

    int totalHealth = curField[1]->GetHealth();
    totalHealth += curField[2]->GetHealth();
    EXPECT_EQ(totalHealth, 3);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    totalHealth = curField[1]->GetHealth();
    totalHealth += curField[2]->GetHealth();
    EXPECT_EQ(totalHealth, 4);
}

// --------------------------------------- MINION - NEUTRAL
// [CS1_069] Fen Creeper - COST:5 [ATK:3/HP:6]
// - Faction: Alliance, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Taunt</b>
// --------------------------------------------------------
// GameTag:
// - TAUNT = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, CS1_069_FenCreeper)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [CS2_117] Earthen Ring Farseer - COST:3 [ATK:3/HP:3]
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Battlecry:</b> Restore 3 Health.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_IF_AVAILABLE = 0
// --------------------------------------------------------
TEST(NeutralExpert1Test, CS2_117_EarthenRingFarseer)
{
    GameConfig config;
    config.player1Class = CardClass::PALADIN;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);
    curPlayer.GetHero()->SetDamage(10);

    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Earthen Ring Farseer"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Earthen Ring Farseer"));
    const auto card3 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card3));
    opField[0]->SetDamage(1);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer,
                 PlayCardTask::MinionTarget(card1, curPlayer.GetHero()));
    EXPECT_EQ(curPlayer.GetHero()->GetHealth(), 23);

    game.Process(curPlayer, PlayCardTask::MinionTarget(card2, card3));
    EXPECT_EQ(opField[0]->GetHealth(), 2);
}

// --------------------------------------- MINION - NEUTRAL
// [CS2_151] Silver Hand Knight - COST:5 [ATK:4/HP:4]
// - Faction: Alliance, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Battlecry:</b> Summon a 2/2 Squire.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, CS2_151_SilverHandKnight)
{
    GameConfig config;
    config.player1Class = CardClass::PALADIN;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Silver Hand Knight"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    EXPECT_EQ(curField[0]->GetAttack(), 4);
    EXPECT_EQ(curField[0]->GetHealth(), 4);
    EXPECT_EQ(curField[1]->GetAttack(), 2);
    EXPECT_EQ(curField[1]->GetHealth(), 2);
}

// --------------------------------------- MINION - NEUTRAL
// [CS2_161] Ravenholdt Assassin - COST:7 [ATK:7/HP:5]
// - Faction: Alliance, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Stealth</b>
// --------------------------------------------------------
// GameTag:
// - STEALTH = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, CS2_161_RavenholdtAssassin)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [CS2_169] Young Dragonhawk - COST:1 [ATK:1/HP:1]
// - Race: Beast, Faction: Horde, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Windfury</b>
// --------------------------------------------------------
// GameTag:
// - WINDFURY = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, CS2_169_YoungDragonhawk)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [CS2_181] Injured Blademaster - COST:3 [ATK:4/HP:7]
// - Faction: Horde, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Battlecry:</b> Deal 4 damage to HIMSELF.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, CS2_181_InjuredBlademaster)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::WARLOCK;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Injured Blademaster"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    EXPECT_EQ(curField[0]->GetHealth(), 3);
}

// --------------------------------------- MINION - NEUTRAL
// [CS2_188] Abusive Sergeant - COST:1 [ATK:1/HP:1]
// - Faction: Alliance, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Battlecry:</b> Give a minion +2 Attack this turn.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// - REQ_TARGET_TO_PLAY = 0
// - REQ_MINION_TARGET = 0
// --------------------------------------------------------
TEST(Expert1CardsGen, CS2_188_AbusiveSergeant)
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::WARLOCK;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();
    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Abusive Sergeant"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Abusive Sergeant"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));
    const auto card4 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));

    game.Process(curPlayer, PlayCardTask::Minion(card3));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card4));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer, PlayCardTask::MinionTarget(card1, card3));
    EXPECT_EQ(curField[0]->GetAttack(), 5);

    game.Process(curPlayer, PlayCardTask::MinionTarget(card2, card4));
    EXPECT_EQ(opField[0]->GetAttack(), 5);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(curField[0]->GetAttack(), 3);
    EXPECT_EQ(opField[0]->GetAttack(), 3);
}

// --------------------------------------- MINION - NEUTRAL
// [CS2_231] Wisp - COST:0 [ATK:1/HP:1]
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
TEST(NeutralExpert1Test, CS2_231_Wisp)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_005] Big Game Hunter - COST:5 [ATK:4/HP:2]
// - Set: Expert1, Rarity: Epic
// --------------------------------------------------------
// Text: <b>Battlecry:</b> Destroy a minion with 7 or more Attack.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// - REQ_MINION_TARGET = 0
// - REQ_TARGET_MIN_ATTACK = 7
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_005_BigGameHunter)
{
    GameConfig config;
    config.player1Class = CardClass::PRIEST;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Big Game Hunter"));
    const auto card2 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Core Hound"));
    const auto card3 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card2));
    EXPECT_EQ(opField.GetCount(), 1);

    game.Process(opPlayer, PlayCardTask::Minion(card3));
    EXPECT_EQ(opField.GetCount(), 2);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer,
                 PlayCardTask::MinionTarget(card1, opPlayer.GetHero()));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 6);

    game.Process(curPlayer, PlayCardTask::MinionTarget(card1, card3));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 6);
    EXPECT_EQ(opField.GetCount(), 2);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card1, card2));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 5);
    EXPECT_EQ(opField.GetCount(), 1);
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_008] Argent Squire - COST:1 [ATK:1/HP:1]
// - Faction: Alliance, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Divine Shield</b>
// --------------------------------------------------------
// GameTag:
// - DIVINE_SHIELD = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_008_ArgentSquire)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_010] Worgen Infiltrator - COST:1 [ATK:2/HP:1]
// - Faction: Alliance, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Stealth</b>
// --------------------------------------------------------
// GameTag:
// - STEALTH = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_010_WorgenInfiltrator)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_012] Bloodmage Thalnos - COST:2 [ATK:1/HP:1]
// - Fac: neutral, Set: expert1, Rarity: legendary
// --------------------------------------------------------
// Text: <b>Spell Damage +1</b>
//       <b>Deathrattle:</b> Draw a card.
// --------------------------------------------------------
// GameTag:
// - ELITE = 1
// - SPELLPOWER = 1
// - DEATHRATTLE = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_012_BloodmageThalnos)
{
    GameConfig config;
    config.player1Class = CardClass::MAGE;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();
    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Bloodmage Thalnos"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Blizzard"));
    const auto card3 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Boulderfist Ogre"));
    const auto card4 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));
    const auto card5 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card3));
    game.Process(opPlayer, PlayCardTask::Minion(card4));
    EXPECT_EQ(opField.GetCount(), 2);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    EXPECT_EQ(curPlayer.currentSpellPower, 1);

    game.Process(curPlayer, PlayCardTask::Spell(card2));
    EXPECT_EQ(opField.GetCount(), 1);
    EXPECT_EQ(opField[0]->GetHealth(), 4);
    EXPECT_EQ(opField[0]->GetGameTag(GameTag::FROZEN), 1);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card5));
    game.Process(opPlayer, AttackTask(card5, card1));
    EXPECT_EQ(curField.GetCount(), 0);
    EXPECT_EQ(curPlayer.currentSpellPower, 0);
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 6);
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_017] Jungle Panther - COST:3 [ATK:4/HP:2]
// - Race: Beast, Faction: Horde, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Stealth</b>
// --------------------------------------------------------
// GameTag:
// - STEALTH = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_017_JunglePanther)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_020] Scarlet Crusader - COST:3 [ATK:3/HP:1]
// - Faction: Alliance, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Divine Shield</b>
// --------------------------------------------------------
// GameTag:
// - DIVINE_SHIELD = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_020_ScarletCrusader)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_021] Thrallmar Farseer - COST:3 [ATK:2/HP:3]
// - Faction: Horde, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Windfury</b>
// --------------------------------------------------------
// GameTag:
// - WINDFURY = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_021_ThrallmarFarseer)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_028] Stranglethorn Tiger - COST:5 [ATK:5/HP:5]
// - Race: Beast, Faction: Alliance, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Stealth</b>
// --------------------------------------------------------
// GameTag:
// - STEALTH = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_028_StranglethornTiger)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_029] Leper Gnome - COST:1 [ATK:1/HP:1]
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Deathrattle:</b> Deal 2 damage to the enemy hero.
// --------------------------------------------------------
// GameTag:
// - DEATHRATTLE = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_029_LeperGnome)
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Leper Gnome"));
    const auto card2 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card2));
    game.Process(opPlayer, AttackTask(card2, card1));
    EXPECT_EQ(opPlayer.GetHero()->GetHealth(), 28);
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_032] Sunwalker - COST:6 [ATK:4/HP:5]
// - Faction: Alliance, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Taunt</b>
//       <b>Divine Shield</b>
// --------------------------------------------------------
// GameTag:
// - TAUNT = 1
// - DIVINE_SHIELD = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_032_Sunwalker)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_033] Windfury Harpy - COST:6 [ATK:4/HP:5]
// - Faction: Alliance, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Windfury</b>
// --------------------------------------------------------
// GameTag:
// - WINDFURY = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_033_WindfuryHarpy)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_043] Twilight Drake - COST:4 [ATK:4/HP:1]
// - Faction: Alliance, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Battlecry:</b> Gain +1 Health for each card in your hand.
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_043_TwilightDrake)
{
    GameConfig config;
    config.player1Class = CardClass::MAGE;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Twilight Drake"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 4);
    EXPECT_EQ(curField.GetCount(), 1);
    EXPECT_EQ(curField[0]->GetHealth(), 5);
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_046] Dark Iron Dwarf - COST:4 [ATK:4/HP:4]
// - Faction: Alliance, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Battlecry:</b> Give a minion +2 Attack this turn.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// - REQ_TARGET_TO_PLAY = 0
// - REQ_MINION_TARGET = 0
// --------------------------------------------------------
TEST(Expert1CardsGen, EX1_046_DarkIronDwarf)
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::WARLOCK;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();
    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Dark Iron Dwarf"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Dark Iron Dwarf"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));
    const auto card4 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));

    game.Process(curPlayer, PlayCardTask::Minion(card3));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card4));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer, PlayCardTask::MinionTarget(card1, card3));
    EXPECT_EQ(curField[0]->GetAttack(), 5);

    game.Process(curPlayer, PlayCardTask::MinionTarget(card2, card4));
    EXPECT_EQ(opField[0]->GetAttack(), 5);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(curField[0]->GetAttack(), 3);
    EXPECT_EQ(opField[0]->GetAttack(), 3);
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_067] Argent Commander - COST:6 [ATK:4/HP:2]
// - Faction: Neutral, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Charge</b>
//       <b>Divine Shield</b>
// --------------------------------------------------------
// GameTag:
// - DIVINE_SHIELD = 1
// - CHARGE = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_067_ArgentCommander)
{
    // Do nothing
}

// ---------------------------------------- MINION - NEUTRAL
// [EX1_095] Gadgetzan Auctioneer - COST:5 [ATK:4/HP:4]
// - Faction: Neutral, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: Whenever you cast a spell, draw a card.
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_095_GadgetzanAuctioneer)
{
    GameConfig config;
    config.player1Class = CardClass::MAGE;
    config.player2Class = CardClass::SHAMAN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Gadgetzan Auctioneer"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Pyroblast"));
    const auto card3 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Lightning Bolt"));

    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 6);
    EXPECT_EQ(opPlayer.GetHandZone().GetCount(), 6);
    game.Process(curPlayer, PlayCardTask::Minion(card1));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 5);
    EXPECT_EQ(opPlayer.GetHandZone().GetCount(), 6);

    game.Process(curPlayer,
                 PlayCardTask::SpellTarget(card2, opPlayer.GetHero()));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 5);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(opPlayer.GetHandZone().GetCount(), 7);
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 5);
    game.Process(opPlayer,
                 PlayCardTask::SpellTarget(card3, curPlayer.GetHero()));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 5);
    EXPECT_EQ(opPlayer.GetHandZone().GetCount(), 6);
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_096] Loot Hoarder - COST:2 [ATK:2/HP:1]
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Deathrattle:</b> Draw a card.
// --------------------------------------------------------
// GameTag:
// - DEATHRATTLE = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_096_LootHoarder)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::HUNTER;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();
    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Loot Hoarder"));
    const auto card2 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 4);
    EXPECT_EQ(curField.GetCount(), 1);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card2));
    EXPECT_EQ(opPlayer.GetHandZone().GetCount(), 6);
    EXPECT_EQ(opField.GetCount(), 1);

    game.Process(opPlayer, AttackTask(card2, card1));
    EXPECT_EQ(curField.GetCount(), 0);
    EXPECT_EQ(opField.GetCount(), 0);
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 5);
    EXPECT_EQ(opPlayer.GetHandZone().GetCount(), 6);
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_097] Abomination - COST:5 [ATK:4/HP:4]
// - Faction: Neutral, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Taunt</b>. <b>Deathrattle:</b> Deal 2
//       damage to ALL characters.
// --------------------------------------------------------
// GameTag:
// - TAUNT = 1
// - DEATHRATTLE = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_097_Abomination)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();
    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Abomination"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Boulderfist Ogre"));
    const auto card4 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));
    const auto card5 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Boulderfist Ogre"));
    const auto card6 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));
    const auto card7 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    game.Process(curPlayer, PlayCardTask::Minion(card2));
    EXPECT_EQ(curField.GetCount(), 2);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card4));
    game.Process(opPlayer, PlayCardTask::Minion(card5));
    EXPECT_EQ(opField.GetCount(), 2);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer, PlayCardTask::Minion(card3));
    EXPECT_EQ(curField.GetCount(), 3);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card6));
    game.Process(opPlayer, PlayCardTask::Minion(card7));
    EXPECT_EQ(opField.GetCount(), 4);

    game.Process(opPlayer, AttackTask(card6, card1));
    game.Process(opPlayer, AttackTask(card7, card1));
    EXPECT_EQ(curPlayer.GetHero()->GetHealth(), 28);
    EXPECT_EQ(curField.GetCount(), 1);
    EXPECT_EQ(curField[0]->GetHealth(), 5);
    EXPECT_EQ(opPlayer.GetHero()->GetHealth(), 28);
    EXPECT_EQ(opField.GetCount(), 1);
    EXPECT_EQ(opField[0]->GetHealth(), 5);
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_102] Demolisher - COST:3 [ATK:1/HP:4]
// - Race: Mechanical, - Faction: Neutral, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: At the start of your turn, deal 2 damage to a random enemy.
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_102_Demolisher)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Demolisher"));
    const auto card2 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Dalaran Mage"));
    const auto card3 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Dalaran Mage"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card2));
    game.Process(opPlayer, PlayCardTask::Minion(card3));

    int totalHealth = opField[0]->GetHealth();
    totalHealth += opField[1]->GetHealth();
    totalHealth += opPlayer.GetHero()->GetHealth();
    EXPECT_EQ(totalHealth, 38);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    totalHealth = opField[0]->GetHealth();
    totalHealth += opField[1]->GetHealth();
    totalHealth += opPlayer.GetHero()->GetHealth();
    EXPECT_EQ(totalHealth, 36);
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_170] Emperor Cobra - COST:3 [ATK:2/HP:3]
// - Race: Beast, Faction: Neutral, Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Poisonous</b>
// --------------------------------------------------------
// GameTag:
// - POISONOUS = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_170_EmperorCobra)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_249] Baron Geddon - COST:7 [ATK:7/HP:5]
// - Race: Elemental, Faction: Neutral, Set: Expert1, Rarity: Legendary
// --------------------------------------------------------
// Text: At the end of your turn, deal 2 damage to ALL other characters.
// --------------------------------------------------------
// GameTag:
// - ELITE = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_249_BaronGeddon)
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::WARLOCK;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();
    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByID("EX1_249"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));
    const auto card3 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));
    const auto card4 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Boulderfist Ogre"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    game.Process(curPlayer, PlayCardTask::Minion(card2));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(curField.GetCount(), 1);
    EXPECT_EQ(curField[0]->GetHealth(), 5);
    EXPECT_EQ(curPlayer.GetHero()->GetHealth(), 28);

    game.Process(opPlayer, PlayCardTask::Minion(card3));
    game.Process(opPlayer, PlayCardTask::Minion(card4));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(opField.GetCount(), 2);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(curField[0]->GetHealth(), 5);
    EXPECT_EQ(opField.GetCount(), 1);
    EXPECT_EQ(opField[0]->GetHealth(), 5);
    EXPECT_EQ(curPlayer.GetHero()->GetHealth(), 26);
    EXPECT_EQ(opPlayer.GetHero()->GetHealth(), 26);
}

// ---------------------------------------- SPELL - WARLOCK
// [EX1_309] Siphon Soul - COST:6
// - Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: Destroy a minion. Restore #3 Health to your hero.
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// - REQ_MINION_TARGET = 0
// --------------------------------------------------------
TEST(WarlockExpert1Test, EX1_309_SiphonSoul)
{
    GameConfig config;
    config.player1Class = CardClass::WARLOCK;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);
    curPlayer.GetHero()->SetDamage(8);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Siphon Soul"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Voidwalker"));

    game.Process(curPlayer, PlayCardTask::Minion(card2));
    EXPECT_EQ(curPlayer.GetHero()->GetHealth(), 22);
    EXPECT_EQ(curField.GetCount(), 1);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card1, card2));
    EXPECT_EQ(curPlayer.GetHero()->GetHealth(), 25);
    EXPECT_EQ(curField.GetCount(), 0);
}

// ---------------------------------------- SPELL - WARLOCK
// [EX1_312] Twisting Nether - COST:8
// - Set: Expert1, Rarity: Epic
// --------------------------------------------------------
// Text: Destroy all minions.
// --------------------------------------------------------
TEST(WarlockExpert1Test, EX1_312_TwistingNether)
{
    GameConfig config;
    config.player1Class = CardClass::WARLOCK;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();
    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Twisting Nether"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Voidwalker"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));
    const auto card4 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));
    const auto card5 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));

    game.Process(curPlayer, PlayCardTask::Minion(card2));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 6);
    EXPECT_EQ(curField.GetCount(), 1);

    game.Process(curPlayer, PlayCardTask::Minion(card3));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 5);
    EXPECT_EQ(curField.GetCount(), 2);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card4));
    EXPECT_EQ(opPlayer.GetHandZone().GetCount(), 7);
    EXPECT_EQ(opField.GetCount(), 1);

    game.Process(opPlayer, PlayCardTask::Minion(card5));
    EXPECT_EQ(opPlayer.GetHandZone().GetCount(), 6);
    EXPECT_EQ(opField.GetCount(), 2);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 5);
    EXPECT_EQ(curField.GetCount(), 0);
    EXPECT_EQ(opPlayer.GetHandZone().GetCount(), 6);
    EXPECT_EQ(opField.GetCount(), 0);
}

// --------------------------------------- MINION - WARLOCK
// [EX1_313] Pit Lord - COST:4 [ATK:5/HP:6]
// - Race: Demon, Set: Expert1, Rarity: Epic
// --------------------------------------------------------
// Text: <b>Battlecry:</b> Deal 5 damage to your hero.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// --------------------------------------------------------
TEST(WarlockExpert1Test, EX1_313_PitLord)
{
    GameConfig config;
    config.player1Class = CardClass::PALADIN;
    config.player2Class = CardClass::WARLOCK;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    const auto card1 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Pit Lord"));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card1));
    EXPECT_EQ(opPlayer.GetHero()->GetHealth(), 25);
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_396] Mogu'shan Warden - COST:4 [ATK:1/HP:7]
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Taunt</b>
// --------------------------------------------------------
// GameTag:
// - TAUNT = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_396_MogushanWarden)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_405] Shieldbearer - COST:1 [ATK:0/HP:4]
// - Faction: Neutral, Set: Expert1, Rarity: Common
// --------------------------------------------------------
// Text: <b>Taunt</b>
// --------------------------------------------------------
// GameTag:
// - TAUNT = 1
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_405_Shieldbearer)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [EX1_563] Malygos - COST:9 [ATK:4/HP:12]
// - Race: Dragon, Faction: Neutral, Set: Expert1, Rarity: Legendary
// --------------------------------------------------------
// Text: <b>Spell Damage +5</b>
// --------------------------------------------------------
// GameTag:
// - ELITE = 1
// - SPELLPOWER = 5
// --------------------------------------------------------
TEST(NeutralExpert1Test, EX1_563_Malygos)
{
    // Do nothing
}

// --------------------------------------- MINION - NEUTRAL
// [NEW1_021] Doomsayer - COST:2 [ATK:0/HP:7]
// - Faction: Neutral, Set: Expert1, Rarity: Epic
// --------------------------------------------------------
// Text: At the start of your turn, destroy ALL minions.
// --------------------------------------------------------
TEST(NeutralExpert1Test, NEW1_021_Doomsayer)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();
    auto& opField = opPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Doomsayer"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Kor'kron Elite"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));
    const auto card4 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Water Elemental"));
    const auto card5 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));

    game.Process(curPlayer, PlayCardTask::Minion(card2));
    game.Process(curPlayer, PlayCardTask::Minion(card3));
    game.Process(curPlayer, PlayCardTask::Minion(card1));
    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 4);
    EXPECT_EQ(curField.GetCount(), 3);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card4));
    game.Process(opPlayer, PlayCardTask::Minion(card5));
    EXPECT_EQ(opPlayer.GetHandZone().GetCount(), 6);
    EXPECT_EQ(opField.GetCount(), 2);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(curPlayer.GetHandZone().GetCount(), 5);
    EXPECT_EQ(curField.GetCount(), 0);
    EXPECT_EQ(opPlayer.GetHandZone().GetCount(), 6);
    EXPECT_EQ(opField.GetCount(), 0);
}

// --------------------------------------- MINION - NEUTRAL
// [NEW1_027] Southsea Captain - COST:3 [ATK:3/HP:3]
// - Race: Pirate, Faction: Neutral, Set: Expert1, Rarity: Epic
// --------------------------------------------------------
// Text: Your other Pirates have +1/+1.
// --------------------------------------------------------
TEST(NeutralExpert1Test, NEW1_027_SouthseaCaptain)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Southsea Captain"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Southsea Captain"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Shieldbearer"));
    const auto card4 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Southsea Captain"));
    const auto card5 = Generic::DrawCard(
        opPlayer, Cards::GetInstance().FindCardByName("Southsea Captain"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    EXPECT_EQ(curField[0]->GetAttack(), 3);
    EXPECT_EQ(curField[0]->GetHealth(), 3);

    game.Process(curPlayer, PlayCardTask::Minion(card3));
    EXPECT_EQ(curField[1]->GetAttack(), 0);
    EXPECT_EQ(curField[1]->GetHealth(), 4);

    game.Process(curPlayer, PlayCardTask::Minion(card4));
    EXPECT_EQ(curField[0]->GetAttack(), 4);
    EXPECT_EQ(curField[0]->GetHealth(), 4);
    EXPECT_EQ(curField[2]->GetAttack(), 4);
    EXPECT_EQ(curField[2]->GetHealth(), 4);

    game.Process(curPlayer, PlayCardTask::Minion(card2));
    EXPECT_EQ(curField[0]->GetAttack(), 5);
    EXPECT_EQ(curField[0]->GetHealth(), 5);
    EXPECT_EQ(curField[1]->GetAttack(), 0);
    EXPECT_EQ(curField[1]->GetHealth(), 4);
    EXPECT_EQ(curField[2]->GetAttack(), 5);
    EXPECT_EQ(curField[2]->GetHealth(), 5);
    EXPECT_EQ(curField[3]->GetAttack(), 5);
    EXPECT_EQ(curField[3]->GetHealth(), 5);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    game.Process(opPlayer, PlayCardTask::Minion(card5));
    EXPECT_EQ(curField[0]->GetAttack(), 5);
    EXPECT_EQ(curField[0]->GetHealth(), 5);
    EXPECT_EQ(curField[1]->GetAttack(), 0);
    EXPECT_EQ(curField[1]->GetHealth(), 4);
    EXPECT_EQ(curField[2]->GetAttack(), 5);
    EXPECT_EQ(curField[2]->GetHealth(), 5);
    EXPECT_EQ(curField[3]->GetAttack(), 5);
    EXPECT_EQ(curField[3]->GetHealth(), 5);
}

// --------------------------------------- MINION - NEUTRAL
// [NEW1_037] Master Swordsmith - COST:2 [ATK:1/HP:3]
// - Set: Expert1, Rarity: Rare
// --------------------------------------------------------
// Text: At the end of your turn, give another random friendly minion +1 Attack.
// --------------------------------------------------------
TEST(NeutralExpert1Test, NEW1_037_MasterSwordsmith)
{
    GameConfig config;
    config.player1Class = CardClass::WARLOCK;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);

    auto& curField = curPlayer.GetFieldZone();

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Master Swordsmith"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    game.Process(curPlayer, PlayCardTask::Minion(card2));
    game.Process(curPlayer, PlayCardTask::Minion(card3));

    int totalAttack = curField[1]->GetAttack();
    totalAttack += curField[2]->GetAttack();
    EXPECT_EQ(totalAttack, 6);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    totalAttack = curField[1]->GetAttack();
    totalAttack += curField[2]->GetAttack();
    EXPECT_EQ(totalAttack, 7);
}