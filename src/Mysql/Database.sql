drop database if exists db1652297;
create database db1652297;

use db1652297;
drop table if exists account;

create table account (
username varchar(30) BINARY not null,
password varchar(100) not null
);
alter table account add primary key(username);

insert into account(username, password) values('Cyanic', MD5('123456'));
insert into account(username, password) values('Casa', MD5('123456'));
insert into account(username, password) values('YYYuna', MD5('123456'));
insert into account(username, password) values('Larry', MD5('123456'));