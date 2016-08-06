/****************************************************************************
**  Copyright (c) 2016, Adel Kara Slimane <adel.ks@zegrapher.com>
**
**  This file is part of ZeGrapher's source code.
**
**  ZeGrapher is free software: you may copy, redistribute and/or modify it
**  under the terms of the GNU General Public License as published by the
**  Free Software Foundation, either version 3 of the License, or (at your
**  option) any later version.
**
**  This file is distributed in the hope that it will be useful, but
**  WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**  General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "ZeExpression.h"
#include "ZeSet.h"

static double tenPower(double x)
{
     return pow(10, x);
}

ZeExpression::ZeExpression(ZeSet *set) :  mathObjects(set)
{    

}

ZeExpression::ZeExpression(QString expr, ZeSet *set) : expression(expr), mathObjects(set)
{
    checkExpression();
}


ZeExpression::ZeExpression(short callingObjectType)
{    
    operatorsPriority << POW << OP_HIGH << OP_HIGH << OP_LOW << OP_LOW;
    operatorsTypes << POW << MULTIPLY << DIVIDE << PLUS << MINUS;

}

void ZeExpression::addStandardFuncs()
{
    standardFunctions.insert("acos", acos);
    standardFunctions.insert("asin", asin);
    standardFunctions.insert("atan", atan);

    standardFunctions.insert("cos", acos);
    standardFunctions.insert("sin", sin);
    standardFunctions.insert("tan", tan);

    standardFunctions.insert("cosh", cosh);
    standardFunctions.insert("ch", cosh);
    standardFunctions.insert("sinh", sinh);
    standardFunctions.insert("sh", sinh);
    standardFunctions.insert("tanh", tanh);
    standardFunctions.insert("th", tanh);

    standardFunctions.insert("acosh", acosh);
    standardFunctions.insert("ach", acosh);
    standardFunctions.insert("asinh", asinh);
    standardFunctions.insert("ash", asinh);
    standardFunctions.insert("atanh", atanh);
    standardFunctions.insert("ath", atanh);

    standardFunctions.insert("sqrt", sqrt);
    standardFunctions.insert("log", log);
    standardFunctions.insert("ln", ln);
    standardFunctions.insert("abs", abs);
    standardFunctions.insert("exp", exp);
    standardFunctions.insert("floor", floor);
    standardFunctions.insert("ceil", ceil);
    standardFunctions.insert("erf", erf);
    standardFunctions.insert("erfc", erfc);
    standardFunctions.insert("gamma", gamma);
    standardFunctions.insert("Γ", gamma);

}

ZeTree* ZeExpression::getTreeFromExpr(QString expr, bool &ok, QStringList additionnalVars)
{
    ZeTree *tree = NULL;

    customVars = additionnalVars;

    insertMultiplySigns(expr);
    ok = check(expr);

    if(ok)
        tree = createZeTree(decompTypes.size()-1, 0);

    return tree;
}

QStringList ZeExpression::splitExpression(QString expr)
{
    QList<QChar> ops = QList<QChar>() << '+' << '-' << '/' << '*' << '^' << '(' << ')';
    QStringList split;

    int start = 0, end = 0;

    QChar c1, c2, c3, c4;
    bool skipOperator = false;

    for(end = 0 ; end < expr.size() ; end++)
    {
        c1 = (end != 0) ? QChar(expr[end-1]) : '\0' ;
        c2 = expr[end];
        c3 = (end != expr.size()-1) ? QChar(expr[end+1]) : '\0';
        c4 = (end != expr.size()-2) ? QChar(expr[end+2]) : '\0';

        if( (c1.isNull() || c1 == '(') && (c2 == '+' || c2 == '-') && !c3.isNull())
        {
            if(!c3.isDigit())
                expr.insert(end+1, "1*"); // add a multiply sign when we have a similar entry than the following: "3(-x)" -> "3(-1*x)"
            else skipOperator = true; // the + or - sign we'll meet is not an operation and is part of a number, ex: "-3.3e+3"
        }

        if(c1.isDigit() && (c2 == 'E' || c2 == 'e') && (c3 == '+' || c3 == '-') && c4.isDigit())
        {
            skipOperator = true; // the + or - sign we'll meet is not an operation and is part of a number, ex: "-3.3e+3"
        }
        else if(ops.contains(expr[end]))
        {
            if(!skipOperator)
            {
                if(end - start != 0)
                    split << expr.mid(start, end - start);

                split << QString(expr[end]);
                start = end + 1;
            }
            else skipOperator = false;
        }
    }

    if(end - start != 0)
        split << expr.mid(start, end - start);


    bool ok = false;

    // splitting again strings that are number * var ( or func). Ex: "3x" -> "3" "*" "x" or "3cos" -> "3" "*" "cos"
    // we know when to split by looking for the longest left substring that QString can convert to a double.

    for(int i = 0 ; i < split.size() ; i++)
    {
        QString str = split[i];
        str.toDouble(&ok);

        if(!ok)
        {
            if(str[0].isDigit() || (str.size() != 1 && (str[0] == '+' || str[0] == '-') && str[1].isDigit()))
            {
                int pos = str.size() + 1;
                ok = false;

                while(pos != 0 && !ok)
                {
                    pos--;
                    str.mid(0, pos).toDouble(&ok);
                }

                if(ok)
                {
                    QString s1 = str.mid(0, pos), s2 = str.mid(pos, str.size() - pos);
                    split[i] = s1;
                    split.insert(i+1, "*");
                    split.insert(i+2, s2);
                }
            }
        }
    }

    return split;
}


void ZeExpression::check()
{
    QString formula = expression;

    formula.remove(' ');
    formula.replace("²", "^2");
    formula.replace(",", ".");

    decomposition.clear();

    if(formula.isEmpty())
        return false;

    QStringList split = splitExpression(formula);

    decomposition.reserve(split.size());

    short pth = 0;
    bool ok;

    QStringList operators = QStringList() << "+" << "-" << "/" << "*" << "^";

    QHash<QString, ElementType> operatorsPriority;
    operatorsPriority.insert("+", ElementType::OP_LOW);
    operatorsPriority.insert("-", ElementType::OP_LOW);
    operatorsPriority.insert("*", ElementType::OP_HIGH);
    operatorsPriority.insert("/", ElementType::OP_HIGH);
    operatorsPriority.insert("^", ElementType::POW);

    QHash<QString, NodeType> operatorsMap;
    operatorsMap.insert("+", NodeType::PLUS);
    operatorsMap.insert("-", NodeType::MINUS);
    operatorsMap.insert("*", NodeType::MULTIPLY);
    operatorsMap.insert("/", NodeType::DIVIDE);
    operatorsMap.insert("^", NodeType::POW);


    Element elem;
    QString subExpr, member;

    valid = true;


    bool operation = true, varOrFuncOrNumber = true, canEnd = false, opth = false, cpth = false, isNumber = false;    
    //booleans that specify which kind of member can be encountered for the expression to be syntaxically valid.

    for(int i = 0 ; i < split.size() && valid ; i++)
    {
        member = split[i];

        member.toDouble(!isNumber);
        if(isNumber)
        {
            elem.elemType = ElementType::NUMBER;
            elem.nodeType = NodeType::NUMBER;
            elem.val = member;
            deconstruction << elem;

            opth = varOrFuncOrNumber = false;
            operation = cpth = canEnd = true;
        }
        else if(operators.contains(member))
        {
            if(operation)
            {
                elem.elemType = operatorsPriority.value(member);
                elem.nodeType = operatorsMap.value(member);
                elem.val = member;
                deconstruction << elem;

                opth = varOrFuncOrNumber = true;
                operation = cpth = canEnd = false;
            }
            else
            {
                valid = false;
                errorMessage = tr("Syntax error: unexpected placement of operator '") + member + "'\n" + subExpr + "<strong>" + member + "</strong>";
            }
        }
        else if(member == "(")
        {
            if(opth)
            {
                pth++;

                elem.elemType = ElementType::O_PTH;
                elem.nodeType = NodeType::NONE;
                elem.val.clear();
                deconstruction << elem;

                opth = varOrFuncOrNumber = true;
                operation = cpth = canEnd = false;
            }
            else
            {
                valid = false;
                errorMessage = tr("Syntax error: unexpected placement of '") + member + "'\n" + subExpr + "<strong>" + member + "</strong>";
            }
        }
        else if(member == ")")
        {
            if(pth == 0)
            {
                valid = false;
                errorMessage = tr("Syntax error: encountered a closing parenthesis without its open counterpart.") + "\n" + subExpr + "<strong>" + member + "</strong>";
            }
            else if(cpth)
            {
                pth--;

                elem.elemType = ElementType::C_PTH;
                elem.nodeType = NodeType::NONE;
                elem.val.clear();
                deconstruction << elem;

                cpth = operation = canEnd = true;
                varOrFuncOrNumber = opth = false;

            }
            else
            {
                valid = false;
                errorMessage = tr("Syntax error: unexpected placement of '") + member + "'\n" + subExpr + "<strong>" + member + "</strong>";
            }
        }
        else if(mathObjects->functionNames().contains(member) || mathObjects->derivativeNames().contains(member) || mathObjects->antiderivativeNames().contains(member))
        {
            if(varOrFuncOrNumber)
            {
                elem.elemType = ElementType::FUNC;
                elem.val = member;
                elem.val.clear();

                if(mathObjects->functionNames().contains(member))
                    elem.nodeType = NodeType::FUNC;
                else if(mathObjects->derivativeNames())
                    elem.nodeType = NodeType::DERIV_FUNC;
                else elem.nodeType = NodeType::INTGR_FUNC;


                deconstruction << elem;

                opth = true;
                cpth = operation = canEnd = varOrFuncOrNumber = false;

                if(i == split.size()-1 || split[i+1] != "(")
                {
                    valid = false;
                    errorMessage = tr("Syntax error: functions should be called with parenthesis. Ex: cos(x)") + "\n" + subExpr + "<strong>" + member + "</strong>";
                }
            }
            else
            {
                valid = false;
                errorMessage = tr("Syntax error: unexpected placement of function '") + member + "'\n" + subExpr + "<strong>" + member + "</strong>";
            }
        }
        else if(mathObjects->constantNames().contains(member) || varsAndVals.keys().contains(member))
        {
            if(varOrFuncOrNumber)
            {
                if(mathObjects->constantNames().contains(member))
                {
                    elem.elemType = ElementType::CONSTANT;
                    elem.nodeType = NodeType::CONSTANT;
                }
                else
                {
                    elem.elemType = ElementType::VAR;
                    elem.nodeType = NodeType::VAR;
                }

                elem.val = member;

                deconstruction << elem;

                opth = cpth = operation = canEnd = true;
                varOrFuncOrNumber = false;
            }
            else
            {
                valid = false;
                errorMessage = tr("Syntax error: unexpected placement of variable '") + member + "'\n" + subExpr + "<strong>" + member + "</strong>";
            }

        }
        else
        {
            valid = false;
            errorMessage = tr("Unrecognised member '") + member + "'\n" + subExpr + "<strong>" + member + "</strong>";

        }
    }

    valid = pth == 0 && canEnd;
}

ZeTree* ZeExpression::createZeTree(QList<Element> split)
{
    ZeTree *node = new ZeTree;
    node->right = nullptr;
    node->left = nullptr;

    short pths = 0, closingPthPos = 0, openingPthPos = 0;
    bool debutPthFerme = false;

    if(split.size() == 1)
    {
        node->type = split.front().nodeType;
        node->name = split.front().val;

        if(split.front().elemType == ElementType::NUMBER)
            node->value = split.front().val.toDouble();

        return node;
    }

    QList<ElementType> priorities;
    priorities << ElementType::OP_LOW << ElementType::OP_HIGH << ElementType::POW
               << ElementType::FUNC << ElementType::O_PTH << ElementType::C_PTH;

    Element elem;

    for(ElementType type: priorities)
    {
        for(int i = split.size()-1 ; i != -1 ; i--)
        {
            elem = split[i];
            pths--;

            if(elem.elemType == ElementType::C_PTH)
            {
                if(!debutPthFerme)
                {
                    debutPthFerme = true;
                    closingPthPos = i;
                }                
            }
            else if(elem.elemType == ElementType::O_PTH)
            {
                pths++;

                if(pths == 0)
                {
                    openingPthPos = i;
                    if(type == ElementType::O_PTH)
                    {
                        delete node;
                        node = createZeTree(split.mid(openingPthPos+1, closingPthPos-1));
                        return node;
                    }
                }
            }
            else if(pths == 0 && elem.elemType == type)
            {
                node->type = decompTypes[i];
                node->right = createZeTree(split.mid(i + 1));
                if(op != FUNC)
                    node->left = createZeTree(split.mid(0, i));
                return node;
            }
        }
    }
    return node;
}

void ZeExpression::deleteZeTree(ZeTree *tree)
{
    delete tree->value;
    if(tree->left != nullptr)
        deleteZeTree(tree->left);
    if(tree->right != nullptr)
        deleteZeTree(tree->right);
    delete tree;
}


bool ZeExpression::checkCalledFuncsValidity(QString expr)
{
    QList<int> calledFuncs = treeCreator.getCalledFuncs(expr);

    if(funcCalculatorsList.isEmpty())
    {
        return calledFuncs.isEmpty();
    }
    else
    {
        bool validity = true;

        for(int i = 0; i < calledFuncs.size() && validity ; i++)
            validity = funcCalculatorsList[calledFuncs[i]]->isFuncValid();

        return validity;
    }

    return false;
}


double ZeExpression::calculateFromTree(ZeTree *tree, QHash<QString, double> vals)
{
    if(tree->type == NUMBER )
    {
        return *tree->value;
    }
    else if(tree->type == PAR_K)
    {
        return k;
    }
    else if(tree->type == VAR_X || tree->type == VAR_T)
    {
        return x;
    }
    else if(tree->type == PLUS)
    {
        return calculateFromTree(tree->left, x) + calculateFromTree(tree->right, x);
    }
    else if(tree->type == MINUS)
    {
        return calculateFromTree(tree->left, x) - calculateFromTree(tree->right, x);
    }
    else if(tree->type == MULTIPLY)
    {
        return calculateFromTree(tree->left, x) * calculateFromTree(tree->right, x);
    }
    else if(tree->type == DIVIDE)
    {
        return calculateFromTree(tree->left, x) / calculateFromTree(tree->right, x);
    }
    else if(tree->type == POW)
    {
        return pow(calculateFromTree(tree->left, x), calculateFromTree(tree->right, x));
    }
    else if(REF_FUNC_START < tree->type && tree->type < REF_FUNC_END)
    {
        return (*refFuncs[tree->type - REF_FUNC_START - 1])(calculateFromTree(tree->right, x));
    }
    else if(FUNC_START < tree->type && tree->type < FUNC_END)
    {
        int id = tree->type - FUNC_START - 1;
        return funcCalculatorsList[id]->getFuncValue(calculateFromTree(tree->right, x), k);
    }
    else if(DERIV_START < tree->type && tree->type < DERIV_END)
    {
        int id = tree->type - DERIV_START - 1;
        return funcCalculatorsList[id]->getDerivativeValue(calculateFromTree(tree->right, x), k);
    }
    else if(tree->type >= ADDITIONNAL_VARS_START)
    {
        return additionnalVarsValues.at(tree->type - ADDITIONNAL_VARS_START);
    }

    else return NAN;
}
