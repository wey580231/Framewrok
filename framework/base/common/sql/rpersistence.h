﻿/*!
 *  @brief     SQL组装
 *  @details   实现对SQL语句的组装，增加灵活性；线程可重入
 *  @file      rpersistence.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note      20180411:wey:增强RSelect用于表联合查询;
 *             20180413:wey:增强RSelect排序、分页查询;
 *             20180414:wey:增加重载函数，减少重复传入参数;
 *                          将函数调整为链式调用，简单直接。
 *             20180422:wey:修复sqlite数据库update操作不支持alias别名
 */
#ifndef RPERSISTENCE_H
#define RPERSISTENCE_H

#include <QString>
#include <QVariant>
#include <QMap>
#include <QVector>
#include <QList>

#include "../../base_global.h"

namespace Base {

	/*!
		@note  条件查询可分为多种组合，如from table where a = 1 and b != 3 or c like '%a%';
		分析sql语句，可将条件分为两种:
		[1]and、or 用于连接多个条件
		[2]=、!=、like、in 用于具体描述某个条件

		类Criteria用于连接多个条件，可通过add()、or()方式对条件进行拼接；
		类Restrictions用于描述具体条件，可通过eq()、gt()、lt()、like()、unlike()、in()等设置不同的条件；

		通过类Criteria可返回最终的条件部分sql语句.

		20210123
		其中两个参数版：用于单张表的关系查询
		带tableName的三参数版：用于多张表关系查询
	*/

	class BASESHARED_EXPORT Restrictions
	{
	public:
		static Restrictions eq(QString key, QVariant value);
		static Restrictions eq(QString tName, QString key, QVariant value);

		static Restrictions gt(QString key, QVariant value);
		static Restrictions gt(QString tName, QString key, QVariant value);

		static Restrictions ge(QString key, QVariant value);
		static Restrictions ge(QString tName, QString key, QVariant value);

		static Restrictions lt(QString key, QVariant value);
		static Restrictions lt(QString tName, QString key, QVariant value);

		static Restrictions le(QString key, QVariant value);
		static Restrictions le(QString tName, QString key, QVariant value);

		static Restrictions ne(QString key, QVariant value);
		static Restrictions ne(QString tName, QString key, QVariant value);

		static Restrictions like(QString key, QVariant value);
		static Restrictions like(QString tName, QString key, QVariant value);

		static Restrictions in(QString key, QVariant value);
		static Restrictions in(QString tName, QString key, QVariant value);

		bool operator<(const Restrictions & src)const;

		QString tableName()const { return tname; }

		QString toSql(QString tableAlais = "", bool isSupportAlias = true)const;
	private:
		enum OperateType
		{
			EQ,             //=
			GT,             //>
			GE,             //>=
			LT,             //<
			LE,             //<=
			NE,             //!=
			LIKE,           //like
			IN              //in
		};

		explicit Restrictions(QString tName, QString key, QVariant value, OperateType type);

		QString name;
		QVariant value;
		QString tname;              /*!< tableName */
		OperateType operation;
	};

	class BASESHARED_EXPORT Criteria
	{
	public:
		explicit Criteria();
		Criteria & add(Restrictions rest);
		Criteria & orr(Restrictions rest);

		int size() { return restricitinons.size(); }

		void clear() { restricitinons.clear(); }

		QString toSql();
		QString toSql(QMap<QString, QString> tableAlias, bool isSupportAlias = true);

	private:
		enum CriteriaType
		{
			CADD,
			COR
		};

		QMap<Restrictions, CriteriaType> restricitinons;
	};

	class SuperCondition
	{
	public:
		SuperCondition() {}

		Criteria& createCriteria() {
			return ctia;
		}

		void clearRestrictions() {
			ctia.clear();
		}

		virtual QString sql() = 0;
		virtual void clear();

		Criteria ctia;

		/*!
		 *  @brief  排序类型
		 */
		enum SOrder {
			ASC,                /*! 升序 */
			DESC                /*! 降序 */
		};

	protected:
		struct OnContion {
			QString t1;             /*!< 连接表A名 */
			QString v1;             /*!< 连接表A中字段名 */
			QString t2;             /*!< 连接表B名 */
			QString v2;             /*!< 连接表B中字段名 */
		};

		struct Keys {
			QString tname;
			QString tkey;
		};

		/*!
		 *  @brief  更新时表示更新字段
		 */
		struct UpdateKeys {
			QString tname;
			QString tKey;
			QVariant value;
		};

		/*!
		 *  @brief 表排序
		 */
		struct Orders {
			QString tName;          /*!< 表名 */
			QString tkey;           /*!< 字段名 */
			SOrder odr;             /*!< 排序 */
		};

		QString odrToString(SOrder so) {
			switch (so) {
			case ASC: return "ASC"; break;
			case DESC: return "DESC"; break;
			default:break;
			}
			return QString("");
		}

	};

	/*!
	 *  @brief 查询语句
	 *  @details 针对普通的SQL进行了对象化操作，支持单表查询、连接查询、排序、分页、多条件查询 @n
	 *  @p 链式调用查询数据信息
	 *  @code
	 *     DataTable::RUser user;
	 *     RSelect rst(user.table);
		   rst.select(user.table,{user.password}).
				createCriteria().
				add(Restrictions::eq(user.table,user.account,request->accountId));
		   rst.sql();
	 *  @endcode
	 */
	class BASESHARED_EXPORT RSelect : public SuperCondition
	{
	public:
		explicit RSelect(std::initializer_list<QString> tNames);
		RSelect(QString tName);
		RSelect & select(const QString & tName, std::initializer_list<QString> keys);
		RSelect & select(const QString & tName);
		RSelect& on(const QString & tName1, const QString key1, const QString tName2, const QString value2);
		bool limit(unsigned int start, unsigned int count);
		RSelect & orderBy(const QString & tName, const QString key, SuperCondition::SOrder odr = SuperCondition::ASC);

		/*!< 内部查询函数 */
		RSelect & avg(QString columnName);
		RSelect & count(QString columnName = "*",bool distinct = false);
		RSelect & first(QString columnName);
		RSelect & last(QString columnName);
		RSelect & max(QString columnName);
		RSelect & min(QString columnName);
		RSelect & sum(QString columnName);
		RSelect & var(QString columnName);

		QString sql();
		void clear();

	private:

		/*!
		 * @brief SQL内置函数
		 */
		enum SQLFunctionType
		{
			F_Avg,				/*!< 平均数 */
			F_Count,			/*!< 统计 */
			F_First,			/*!< 返回在指定的域中第一个记录的值 */
			F_Last,				/*!< 返回在指定的域中最后一个记录的值 */
			F_Max,				/*!< 返回某列的最高值 */
			F_Min,				/*!< 返回某列的最低值 */
			F_Sum,				/*!< 返回某列的总和 */
			F_Var				/*!< 返回某列的平均值 */
		};

	private:
		QMap<QString, QString> tableNames;          /*!< 所有连接表名 */
		QVector<Keys> selectedKeys;                 /*!< 查询结果信息 */
		QVector<OnContion> onCondtions;             /*!< 连接查询时的条件信息 */
		QVector<Orders> sortOrders;                 /*!< 排序列表 */

		unsigned int limitStart, limitCount;        /*!< 范围查询,查看开始位置，查询的条数 */
		bool isSetLimit;                            /*!< 是否设置范围查询 */

		bool isSetFunction;							/*!< 是否为SQL函数模式，开启此模式后如limit等功能就无法使用了 */
		SQLFunctionType m_functionType;				/*!< 内置函数类型 */		
		QString m_funcColumn;						/*!< SQL函数绑定的列名 */
		bool isDistinct;							/*!< 是否去除重复列，count函数有效 */
	};

	/*!
	 *  @brief  插入语句
	 *  @details 支持单表插入
	 *  @p 链式插入数据记录
	 *  @code
	 *  DataTable::RequestCache rc;
		RPersistence rps(rc.table);
		rps.insert({{rc.id,RUtil::UUID()},
				   {rc.account,accountId},
				   {rc.operateId,operateId},
				   {rc.type,type}});
		rps.sql();
	 *  @endcode
	 */
	class BASESHARED_EXPORT RPersistence : public SuperCondition
	{
	public:
		explicit RPersistence(const QString tableName);
		RPersistence & insert(const QString key, QVariant value);
		RPersistence & insert(std::vector<std::pair<QString, QVariant>> list);
		QString sql();

	private:
		QString tableName;
		QMap<QString, QVariant> maps;
	};

	/*!
	 *  @brief  更新语句
	 *  @details 支持单表、多表连接更新，支持设置多个更新条件。
	 *  @p 链式连接更新数据
	 *  @code
	 *  DataTable::RGroup_User rgu;
		DataTable::RUser ru;

		RUpdate rpd({rgu.table,ru.table});
		rpd.update(rgu.table,rgu.groupId,request->groupId).
				on(rgu.table,rgu.userId,ru.table,ru.id).
				createCriteria().
				add(Restrictions::eq(rgu.table,rgu.groupId,request->oldGroupId)).
				add(Restrictions::eq(ru.table,ru.account,request->user.accountId));
		rpd.sql();
	 *  @endcode
	 */
	class BASESHARED_EXPORT RUpdate : public SuperCondition
	{
	public:
		explicit RUpdate(std::initializer_list<QString> tNames);
		RUpdate(const QString tName);
		RUpdate & enableAlias(bool flag);
		RUpdate &update(const QString tName, const QString key, QVariant value);
		RUpdate &update(const QString tName, std::vector<std::pair<QString, QVariant>> list);
		RUpdate& on(const QString & tName1, const QString key1, const QString tName2, const QString value2);
		QString sql();

	private:
		QMap<QString, QString> tableNames;                   /*!< 所有连接表名 */
		QVector<OnContion> onCondtions;                     /*!< 连接更新时的条件信息 */
		QVector<UpdateKeys> updateKeys;
		bool isAlias;                                       /*!< 是否支持别名，sqlite更新操作不支持alias，mysql则支持 */
	};

	/*!
	 *  @brief  删除语句
	 *  @details 支持单表删除
	 *  @p 链式删除数据记录
	 *  @code
	 *  DataTable::RGroup_User rgu;
		RDelete rde(rgu.table);
		rde.createCriteria()
			.add(Restrictions::eq(rgu.table,rgu.groupId,request->groupId))
			.add(Restrictions::eq(rgu.table,rgu.userId,otherSideUserInfo.uuid));
		rde.sql();
	 *  @endcode
	 */
	class BASESHARED_EXPORT RDelete : public SuperCondition
	{
	public:
		explicit RDelete(const QString tableName);
		QString sql();

	private:
		QString tableName;
	};

} //namespace Base

#endif // RPERSISTENCE_H
