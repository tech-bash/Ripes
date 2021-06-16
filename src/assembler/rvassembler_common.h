#pragma once

#include "../isa/rvisainfo_common.h"
#include "assembler.h"

namespace Ripes {
namespace Assembler {

#define BType(name, funct3)                                                                              \
    std::shared_ptr<Instruction>(new Instruction(                                                        \
        Opcode(name, {OpPart(0b1100011, 0, 6), OpPart(funct3, 12, 14)}),                                 \
        {std::make_shared<Reg>(isa, 1, 15, 19, "rs1"), std::make_shared<Reg>(isa, 2, 20, 24, "rs2"),     \
         std::make_shared<Imm>(                                                                          \
             3, 13, Imm::Repr::Signed,                                                                   \
             std::vector{ImmPart(12, 31, 31), ImmPart(11, 7, 7), ImmPart(5, 25, 30), ImmPart(1, 8, 11)}, \
             Imm::SymbolType::Relative)}))

#define ITypeCommon(opcode, name, funct3)                                                                          \
    std::shared_ptr<Instruction>(                                                                                  \
        new Instruction(Opcode(name, {OpPart(opcode, 0, 6), OpPart(funct3, 12, 14)}),                              \
                        {std::make_shared<Reg>(isa, 1, 7, 11, "rd"), std::make_shared<Reg>(isa, 2, 15, 19, "rs1"), \
                         std::make_shared<Imm>(3, 12, Imm::Repr::Signed, std::vector{ImmPart(0, 20, 31)})}))

#define IType(name, funct3) ITypeCommon(RVISA::OPIMM, name, funct3)
#define IType32(name, funct3) ITypeCommon(RVISA::OPIMM32, name, funct3)

#define LoadType(name, funct3)                                                                                     \
    std::shared_ptr<Instruction>(                                                                                  \
        new Instruction(Opcode(name, {OpPart(0b0000011, 0, 6), OpPart(funct3, 12, 14)}),                           \
                        {std::make_shared<Reg>(isa, 1, 7, 11, "rd"), std::make_shared<Reg>(isa, 3, 15, 19, "rs1"), \
                         std::make_shared<Imm>(2, 12, Imm::Repr::Signed, std::vector{ImmPart(0, 20, 31)})}))

#define IShiftTypeCommon(name, opcode, funct3, funct7, immEnd)                                                     \
    std::shared_ptr<Instruction>(                                                                                  \
        new Instruction(Opcode(name, {OpPart(opcode, 0, 6), OpPart(funct3, 12, 14), OpPart(funct7, 25, 31)}),      \
                        {std::make_shared<Reg>(isa, 1, 7, 11, "rd"), std::make_shared<Reg>(isa, 2, 15, 19, "rs1"), \
                         std::make_shared<Imm>(3, 5, Imm::Repr::Unsigned, std::vector{ImmPart(0, 20, immEnd)})}))

#define IShiftType32(name, opcode, funct3, funct7) IShiftTypeCommon(name, opcode, funct3, funct7, 24)
#define IShiftType64(name, opcode, funct3, funct7) IShiftTypeCommon(name, opcode, funct3, funct7, 25)

#define RType(name, funct3, funct7)                                                                                   \
    std::shared_ptr<Instruction>(                                                                                     \
        new Instruction(Opcode(name, {OpPart(RVISA::OPIMM32, 0, 6), OpPart(funct3, 12, 14), OpPart(funct7, 25, 31)}), \
                        {std::make_shared<Reg>(isa, 1, 7, 11, "rd"), std::make_shared<Reg>(isa, 2, 15, 19, "rs1"),    \
                         std::make_shared<Reg>(isa, 3, 20, 24, "rs2")}))

#define SType(name, funct3)                                                                                   \
    std::shared_ptr<Instruction>(new Instruction(                                                             \
        Opcode(name, {OpPart(0b0100011, 0, 6), OpPart(funct3, 12, 14)}),                                      \
        {std::make_shared<Reg>(isa, 3, 15, 19, "rs1"),                                                        \
         std::make_shared<Imm>(2, 12, Imm::Repr::Signed, std::vector{ImmPart(5, 25, 31), ImmPart(0, 7, 11)}), \
         std::make_shared<Reg>(isa, 1, 20, 24, "rs2")}))

#define UType(name, opcode)                                          \
    std::shared_ptr<Instruction>(                                    \
        new Instruction(Opcode(name, {OpPart(opcode, 0, 6)}),        \
                        {std::make_shared<Reg>(isa, 1, 7, 11, "rd"), \
                         std::make_shared<Imm>(2, 32, Imm::Repr::Hex, std::vector{ImmPart(0, 12, 31)})}))

#define JType(name, opcode)                                                                                  \
    std::shared_ptr<Instruction>(new Instruction(                                                            \
        Opcode(name, {OpPart(opcode, 0, 6)}),                                                                \
        {std::make_shared<Reg>(isa, 1, 7, 11, "rd"),                                                         \
         std::make_shared<Imm>(                                                                              \
             2, 21, Imm::Repr::Signed,                                                                       \
             std::vector{ImmPart(20, 31, 31), ImmPart(12, 12, 19), ImmPart(11, 20, 20), ImmPart(1, 21, 30)}, \
             Imm::SymbolType::Relative)}))

#define JALRType(name)                                                                                             \
    std::shared_ptr<Instruction>(                                                                                  \
        new Instruction(Opcode(name, {OpPart(0b1100111, 0, 6), OpPart(0b000, 12, 14)}),                            \
                        {std::make_shared<Reg>(isa, 1, 7, 11, "rd"), std::make_shared<Reg>(isa, 2, 15, 19, "rs1"), \
                         std::make_shared<Imm>(3, 12, Imm::Repr::Signed, std::vector{ImmPart(0, 20, 31)})}))

#define RegTok PseudoInstruction::reg()
#define ImmTok PseudoInstruction::imm()
#define CreatePseudoInstruction
#define _PseudoExpandFuncSyms(line, symbols) \
    [](const PseudoInstruction&, const TokenizedSrcLine& line, const SymbolMap& symbols)

#define _PseudoExpandFunc(line) [](const PseudoInstruction&, const TokenizedSrcLine& line, const SymbolMap&)

#define PseudoLoad(name)                                                                                               \
    std::shared_ptr<PseudoInstruction>(new PseudoInstruction(                                                          \
        name, {RegTok, ImmTok}, _PseudoExpandFunc(line) {                                                              \
            LineTokensVec v;                                                                                           \
            v.push_back(LineTokens() << Token("auipc") << line.tokens.at(1) << Token(line.tokens.at(2), "%pcrel_hi")); \
            v.push_back(LineTokens() << name << line.tokens.at(1)                                                      \
                                     << Token(QString("(%1 + 4)").arg(line.tokens.at(2)), "%pcrel_lo")                 \
                                     << line.tokens.at(1));                                                            \
            return v;                                                                                                  \
        }))

// The sw is a pseudo-op if a symbol is given as the immediate token. Thus, if we detect that
// a number has been provided, then abort the pseudo-op handling.
#define PseudoStore(name)                                                                                              \
    std::shared_ptr<PseudoInstruction>(new PseudoInstruction(                                                          \
        name, {RegTok, ImmTok, RegTok}, _PseudoExpandFunc(line) {                                                      \
            bool canConvert;                                                                                           \
            getImmediate(line.tokens.at(2), canConvert);                                                               \
            if (canConvert) {                                                                                          \
                return PseudoExpandRes(Error(0, "Unused; will fallback to non-pseudo op sw"));                         \
            }                                                                                                          \
            LineTokensVec v;                                                                                           \
            v.push_back(LineTokens() << Token("auipc") << line.tokens.at(3) << Token(line.tokens.at(2), "%pcrel_hi")); \
            v.push_back(LineTokens() << name << line.tokens.at(1)                                                      \
                                     << Token(QString("(%1 + 4)").arg(line.tokens.at(2)), "%pcrel_lo")                 \
                                     << line.tokens.at(3));                                                            \
            return PseudoExpandRes(v);                                                                                 \
        }))

}  // namespace Assembler
}  // namespace Ripes
