ใช่ครับ สรุปคือ **ไม่ต้องใช้ Machine Learning จริง** แต่ใน Unreal มีเครื่องมือหลายตัวที่ช่วยทำให้ NPC/Enemy ดูฉลาด มีชีวิต และ maintain ง่ายขึ้นมาก

สำหรับ roadmap เกมคุณ ผมจะแบ่งเป็น 3 กลุ่มครับ:

> **1. เครื่องมือทำ AI behavior**  
> **2. เครื่องมือทำ data-driven memory/dialogue**  
> **3. เครื่องมือ debug/simulation เพื่อคุมระบบไม่ให้เละ**

---

# ชุดเครื่องมือที่เหมาะกับเกมคุณที่สุด

## 1. Gameplay Tags — แกนหลักของ Memory / Faction / Quest

อันนี้ควรใช้แน่นอนครับ

ใช้แทน boolean กระจัดกระจาย เช่น:

```text
Memory.PlayerSavedMe
Memory.PlayerFledDuringAttack
World.FirstNight.Completed
Faction.Vampire.Feral
Weakness.Fire
EnemyTrait.Hungry
```

Gameplay Tags ใน Unreal เป็นระบบ label แบบ hierarchical ที่ใช้ติดกับ object แล้วเอาไปประเมิน gameplay logic ได้ เหมาะมากกับระบบ Memory, Dialogue condition, Faction state และ Quest state ของคุณ ([Epic Games Developers](https://dev.epicgames.com/documentation/unreal-engine/using-gameplay-tags-in-unreal-engine?utm_source=chatgpt.com "Using Gameplay Tags in Unreal Engine"))

**ใช้กับอะไรในเกมคุณ:**

```text
NPC Memory
Enemy Memory
Dialogue Condition
Faction Relation
Quest State
World Event
Weakness / Resistance
```

นี่คือเครื่องมือสำคัญที่สุดของระบบ data-driven ทั้งหมด

---

## 2. Data Assets / Data Tables — Dialogue Bank, NPC Bank, Enemy Bank

ส่วนนี้คือคำตอบของปัญหา “Blueprint manual input หนักเกินไป”

ให้เก็บข้อมูลพวกนี้เป็น asset/data:

```text
Dialogue Bank
NPC Profile
Household Profile
Enemy Archetype
Spawn Rule
Nemesis Return Rule
Faction Relation
Memory Seed
```

Unreal Asset Manager รองรับ asset หลายประเภท รวมถึง Data Assets ที่เป็น instance ของ `UDataAsset` ซึ่งเหมาะกับการจัดการ non-Blueprint data ของระบบใหญ่ ๆ ([Epic Games Developers](https://dev.epicgames.com/documentation/unreal-engine/asset-management-in-unreal-engine?utm_source=chatgpt.com "Asset Management in Unreal Engine"))

**แนวทางสำหรับคุณ:**

ใช้ **Data Asset** กับข้อมูลที่มีโครงสร้างซับซ้อน เช่น:

```text
DA_DialogueBank_MotherNPC
DA_EnemyArchetype_FeralVampireAmbusher
DA_NPCGenerationPreset_Emberwall
```

ใช้ **Data Table** กับข้อมูลจำนวนมากแบบตาราง เช่น:

```text
DT_Names_Human
DT_VillagerBarks
DT_VampireTaunts
DT_MemoryReactionLines
```

---

## 3. StateTree — เหมาะมากกับ AI แบบเป็น State ชัด ๆ

สำหรับเกมคุณ ผมแนะนำให้ดู **StateTree** จริงจังครับ

StateTree เป็น hierarchical state machine ที่ผสมแนวคิด selector จาก Behavior Tree กับ state/transition จาก State Machine ทำให้ logic ยืดหยุ่นและเป็นระเบียบ ([Epic Games Developers](https://dev.epicgames.com/documentation/unreal-engine/overview-of-state-tree-in-unreal-engine?utm_source=chatgpt.com "Overview of State Tree in Unreal Engine"))

เหมาะกับ AI ที่มี state แบบนี้:

```text
Villager:
Idle
Work
GoHome
HideAtNight
Panic
ReactToEvent
TalkToPlayer

Feral Vampire:
Lurk
Stalk
Ambush
Attack
Retreat
ReturnToLair
NemesisTaunt
```

สำหรับ roadmap ของคุณ ผมมองว่า **StateTree เหมาะกับ high-level behavior** มาก เช่น NPC daily routine, villager panic, enemy state, vampire stalk/ambush/retreat

---

## 4. Behavior Tree + Blackboard — เหมาะกับ Combat AI แบบคลาสสิก

Behavior Tree ใน Unreal ใช้สร้าง AI ของ NPC ได้ และทำงานร่วมกับ Blackboard ซึ่งทำหน้าที่เป็น “brain” หรือที่เก็บข้อมูลสำหรับ tree นั้น ๆ ([Epic Games Developers](https://dev.epicgames.com/documentation/unreal-engine/behavior-trees-in-unreal-engine?utm_source=chatgpt.com "Behavior Trees in Unreal Engine"))

ผมแนะนำแบบนี้:

```text
StateTree = คุม state ใหญ่
Behavior Tree = คุม combat/tactical behavior
Blackboard = เก็บ target, last known location, danger level, current goal
```

ตัวอย่าง:

```text
StateTree: Vampire อยู่ใน State "Attack"
↓
Behavior Tree: เลือกว่าจะ melee, dodge, flank, retreat, call minion
```

ถ้าเริ่ม MVP ผมว่าใช้ **StateTree ก่อน** ก็พอ แล้วค่อยเพิ่ม Behavior Tree เมื่อ combat ซับซ้อนขึ้น

---

## 5. AI Perception — ให้ Enemy เห็น/ได้ยิน/รับรู้ผู้เล่น

AI Perception Component ใช้ทำให้ AI รับรู้สิ่งต่าง ๆ เช่น การเห็นหรือได้ยิน actor แล้วเอาข้อมูลนั้นไปตอบสนองต่อได้ ([Epic Games Developers](https://dev.epicgames.com/documentation/unreal-engine/ai-perception-in-unreal-engine?utm_source=chatgpt.com "AI Perception in Unreal Engine"))

ใช้กับเกมคุณได้ดีมาก:

```text
Vampire เห็นผู้เล่นตอนกลางคืน
Hunter ได้ยินเสียงต่อสู้
Villager เห็นศพแล้ว panic
Werewolf ได้กลิ่น/รับรู้การล้ำเขต
```

สำหรับ MVP:

```text
Sight
Hearing
Damage Sense
```

แค่นี้พอครับ ไม่ต้องทำ sense แปลก ๆ เยอะตั้งแต่แรก

---

## 6. EQS — ให้ AI เลือกตำแหน่งฉลาดขึ้น

EQS หรือ Environment Query System ใช้ query สภาพแวดล้อมเพื่อเอาข้อมูลไปช่วย AI ตัดสินใจ เช่น หาจุดหลบ หาจุดซุ่ม หาจุดหนี หรือจุดที่เห็นผู้เล่นดีที่สุด ([Epic Games Developers](https://dev.epicgames.com/documentation/unreal-engine/environment-query-system-in-unreal-engine?utm_source=chatgpt.com "Environment Query System in Unreal Engine"))

เหมาะกับระบบแบบนี้:

```text
Vampire หา shadow spot สำหรับซุ่ม
Enemy หา escape point ตอนเลือดต่ำ
Hunter หาจุดยิงจากระยะปลอดภัย
Werewolf หาเส้นทาง flank
Villager หาที่ซ่อนตอนกลางคืน
```

นี่จะช่วยให้ AI ดูฉลาดโดยไม่ต้องใช้ ML เลย

---

## 7. Navigation System / NavMesh — พื้นฐานการเดินของ AI

Navigation System ของ Unreal รองรับการสร้าง NavMesh ทั้งแบบ Static, Dynamic และ Dynamic Modifiers Only รวมถึงมีระบบหลบหลีก agent อย่าง RVO และ Detour Crowd Manager ([Epic Games Developers](https://dev.epicgames.com/documentation/unreal-engine/navigation-system-in-unreal-engine?utm_source=chatgpt.com "Navigation System in Unreal Engine"))

สำหรับเกม top-down ของคุณ สำคัญมาก เพราะมี:

```text
click-to-move
NPC เดินในหมู่บ้าน
Enemy ไล่ล่า
Villager หนี
Patrol route
Return to lair
```

เริ่มจาก NavMesh ธรรมดาก่อน แล้วค่อยเพิ่ม dynamic navigation ถ้าโลกมีประตู สิ่งกีดขวาง หรือ event ที่เปลี่ยนทางเดิน

---

## 8. Smart Objects — ทำให้หมู่บ้านดูมีชีวิต

Smart Objects คือ object ใน level ที่ AI Agent และผู้เล่นสามารถ interact ได้ โดยตัว object เก็บข้อมูลที่จำเป็นสำหรับการใช้งานไว้เอง ([Epic Games Developers](https://dev.epicgames.com/documentation/unreal-engine/smart-objects-in-unreal-engine---overview?utm_source=chatgpt.com "Smart Objects in Unreal Engine - Overview"))

อันนี้เหมาะกับ Emberwall มากครับ

ตัวอย่าง Smart Objects:

```text
Well / บ่อน้ำ
Work Bench
Cooking Fire
Bed
Watch Tower
Shrine
Market Stall
Gate Lever
Corpse Burn Pit
Hiding Spot
```

แทนที่จะเขียน logic ว่า NPC แต่ละคนต้องไปตรงไหนเอง คุณให้โลกบอกว่า:

```text
ตรงนี้คือจุดตักน้ำ
ตรงนี้คือจุดนอน
ตรงนี้คือจุดซ่อน
ตรงนี้คือจุดทำงาน
```

NPC ก็เลือกใช้ตาม role/state ได้

---

# เครื่องมือสำหรับช่วงหลัง ไม่ต้องรีบใช้

## 9. Mass Entity / Mass Gameplay — สำหรับ NPC จำนวนมาก

MassEntity เป็น framework แบบ data-oriented สำหรับคำนวณ gameplay จำนวนมาก และ MassGameplay มีระบบเกี่ยวกับ world representation, spawning, LOD, replication และ StateTree ([Epic Games Developers](https://dev.epicgames.com/documentation/unreal-engine/mass-entity-in-unreal-engine?utm_source=chatgpt.com "Mass Entity in Unreal Engine"))

แต่สำหรับคุณตอนนี้:

> **ยังไม่ต้องใช้ Mass ครับ**

ใช้เมื่อมี NPC/ฝูงจำนวนเยอะจริง ๆ เช่น:

```text
เมืองใหญ่มีชาวบ้านเดินจำนวนมาก
ฝูง zombie
crowd simulation
คาราวาน
background NPC จำนวนมาก
```

สำหรับ Emberwall 21 households ยังใช้ Actor ปกติ + LOD ง่าย ๆ ได้ก่อน

---

## 10. Gameplay Ability System — ใช้ตอน combat/effect เริ่มใหญ่

อันนี้ยังไม่ต้องรีบ แต่ถ้าต่อไปเกมมี:

```text
Fire damage
Silver damage
UV burn
Vampire regeneration
Bleeding
Fear
Curse
Infection
Stamina
Status effects
```

ค่อยพิจารณา GAS หรือทำระบบ effect เองแบบเบาก่อนก็ได้

สำหรับ MVP ผมแนะนำทำ **Damage/Status Component แบบง่าย** ก่อน

---

# เครื่องมือ Debug ที่ควรมีตั้งแต่เนิ่น ๆ

## 11. Visual Logger

Visual Logger ใช้บันทึกและแสดง gameplay state ในเชิงภาพ ช่วยดูย้อนหลังใน Editor ได้ เหมาะกับ bug ที่ดูจาก log ธรรมดายาก ([Epic Games Developers](https://dev.epicgames.com/documentation/unreal-engine/visual-logger-in-unreal-engine?utm_source=chatgpt.com "Visual Logger in Unreal Engine"))

ใช้ debug สิ่งนี้ได้ดีมาก:

```text
AI เห็นผู้เล่นตอนไหน
Enemy เลือกจุดซุ่มผิดไหม
Vampire หนีเพราะอะไร
NPC ได้ memory อะไรหลัง event
Spawn Director เลือก enemy ตัวไหน
```

---

## 12. Gameplay Debugger

Gameplay Debugger แสดงข้อมูล runtime เป็น overlay ใน viewport และสามารถขยายเพื่อ debug ข้อมูลเฉพาะเกมเราได้ ([Epic Games Developers](https://dev.epicgames.com/documentation/unreal-engine/using-the-gameplay-debugger-in-unreal-engine?utm_source=chatgpt.com "Using the Gameplay Debugger in Unreal Engine"))

ผมแนะนำให้คุณทำ debug category ของตัวเอง เช่น:

```text
DW Memory
DW Dialogue
DW Faction
DW Enemy
DW Spawn
```

เวลาเล็ง NPC แล้วเห็นทันที:

```text
NPC: Mara
Memory: PlayerSavedMyChild
Trust: 80
Fear: 40
Current Dialogue Line: Mother_Thanks_01
Quest Hook: RescueMissingVillager
```

อันนี้จะช่วยประหยัดเวลามากครับ

---

## 13. Data Validation

Data Validation plugin ใน Unreal ใช้ validate asset ด้วย ruleset ที่เขียนเองได้ เช่น ตรวจ naming convention, performance budget หรือ dependency loop ([Epic Games Developers](https://dev.epicgames.com/documentation/unreal-engine/data-validation-in-unreal-engine?utm_source=chatgpt.com "Data Validation in Unreal Engine"))

เหมาะกับระบบ Dialogue Bank มาก เพราะต่อไปข้อมูลจะเยอะมาก

ตัวอย่าง validation:

```text
DialogueLine ต้องมี LineId
RequiredTags ต้องไม่ว่างผิดปกติ
SpeakerId ต้องมี NPC จริง
EnemyArchetype ต้องมี Weakness อย่างน้อย 1 อย่าง
NPC Profile ต้องมี SettlementId
Relationship ต้องไม่ชี้ไป NPC ที่ไม่มีอยู่
```

ถ้าไม่มี validation ระบบ data-driven จะพังเงียบ ๆ ง่ายมาก

---

# ชุดเครื่องมือที่ผมแนะนำตาม Phase

## Phase 3.5: Data Foundation

ใช้:

```text
Gameplay Tags
Data Assets
Data Tables
Asset Manager
Data Validation
```

เป้าหมายคือทำให้ข้อมูลไม่กระจัดกระจายใน Blueprint

---

## Phase 3.7: Character Memory Component

ใช้:

```text
Gameplay Tags
Memory Component
World Event Subsystem
Gameplay Debugger
Visual Logger
```

ให้ NPC/Enemy จำ event ได้ก่อน

---

## Phase 3.8: Dialogue Bank + Dialogue Selector

ใช้:

```text
Data Asset / Data Table
Gameplay Tags
Dialogue Subsystem
Data Validation
```

ให้บทพูดเลือกจาก condition ไม่ใช่ต่อ Blueprint เอง

---

## Phase 3.9–3.11: Settlement + Auto-generated NPC

ใช้:

```text
NPC Registry
Settlement Registry
Relationship Graph
Data Assets
Smart Objects
StateTree
```

ให้ NPC มี profile, household, role, relation และ routine เบื้องต้น

---

## Phase 3.12–3.13: Enemy Profile + Nemesis-lite

ใช้:

```text
Enemy Roster
Spawn Director
Gameplay Tags
StateTree
AI Perception
EQS
Visual Logger
```

ให้ Enemy มี memory, territory, return rules และ behavior ที่เปลี่ยนตามอดีต

---

## Phase 9: Vampire AI

ใช้:

```text
StateTree
Behavior Tree / Blackboard ถ้าต้องการ tactical combat
AI Perception
EQS
NavMesh
Dialogue Bank
Enemy Memory
```

นี่จะทำให้ Vampire ไม่ใช่แค่เดินมาตี แต่มี loop:

```text
Lurk → Stalk → Ambush → Attack → Retreat → Remember → Return
```

---

# Stack ที่ผมแนะนำสำหรับคุณ

ถ้าให้เลือกเป็นชุดเดียว ผมแนะนำแบบนี้ครับ:

```text
Core Data:
Gameplay Tags
Data Assets
Data Tables
Data Validation

Core AI:
StateTree
AI Perception
EQS
Navigation System
Smart Objects

Combat AI later:
Behavior Tree + Blackboard
Damage/Status Component หรือ GAS ในอนาคต

World Simulation:
World Event Subsystem
Memory Component
Relationship Graph
NPC Registry
Enemy Roster
Spawn Director

Debug:
Gameplay Debugger
Visual Logger
```

---

# อะไรควรทำก่อนที่สุด?

ผมแนะนำลำดับนี้ครับ:

```text
1. Gameplay Tag Taxonomy
2. Memory Component
3. Dialogue Bank DataAsset
4. Dialogue Selector
5. Gameplay Debugger: แสดง Memory/Dialogue
6. NPC Profile + Registry
7. Relationship Graph
8. Enemy Profile + Roster
9. StateTree สำหรับ Vampire basic behavior
10. EQS สำหรับ ambush/retreat point
```

อย่าเริ่มจาก Behavior Tree ใหญ่ ๆ หรือระบบ NPC routine ใหญ่ทันทีครับ  
ให้เริ่มจาก “จำได้ → พูดเปลี่ยน → debug ได้” ก่อน

---

# สรุปสั้นที่สุด

เครื่องมือที่เหมาะกับ AI เกมนี้ไม่ใช่ Machine Learning แต่คือ:

> **Gameplay Tags + Data Assets + StateTree + AI Perception + EQS + Smart Objects + Debug Tools**

ชุดนี้จะช่วยให้คุณสร้าง NPC/Enemy ที่:

```text
มี memory
มี condition-based dialogue
มี routine
รับรู้ผู้เล่น
เลือกจุดซุ่ม/หนีได้
ใช้สิ่งของในหมู่บ้านได้
กลับมาเป็น nemesis ได้
debug ได้จริง
```

และที่สำคัญคือยังคุม scope ได้ เหมาะกับ solo/indie dev มากกว่าเอา AI learning จริงเข้ามาตั้งแต่ต้นครับ.