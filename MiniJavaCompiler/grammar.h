#pragma once

class IVisitor;

class IHost {
public:
	virtual ~IHost( ) {
	}

	virtual void Accept( IVisitor* visitor ) const = 0;
};

// Корневой интерфейс программы
class IProgram : public IHost {
public:
	virtual ~IProgram() {}
};

// Главный класс
class IMainClass : public IHost {
public:
	virtual ~IMainClass() {}
};

// Список классов
class IClassDeclList : public IHost {
public:
	virtual ~IClassDeclList() {}
};

// Описание класса
class IClassDecl : public IHost {
public:
	virtual ~IClassDecl() {}
};

// Список объявлений переменных
class IVarDeclList : public IHost {
public:
	virtual ~IVarDeclList() {}
};

// Объявление переменной
class IVarDecl : public IHost {
public:
	virtual ~IVarDecl() {}
};

// Список методов класса
class IMethodDeclList : public IHost {
public:
	virtual ~IMethodDeclList() {}
};

// Метод класса
class IMethodDecl : public IHost {
public:
	virtual ~IMethodDecl() {}
};

// Список параметров
class IFormalList : public IHost {
public:
	virtual ~IFormalList() {}
};

// Тип
class IType : public IHost {
public:
	virtual ~IType() {}
};

// Последовательность команд
class IStatementList : public IHost {
public:
	virtual ~IStatementList() {}
};

// Одна команда (или блок)
class IStatement : public IHost {
public:
	virtual ~IStatement() {}
};

// Последовательность выражений (аргументы функции)
class IExpList : public IHost {
public:
	virtual ~IExpList() {}
};

// Арифметическое выражение
class IExp : public IHost {
public:
	virtual ~IExp() {}
};