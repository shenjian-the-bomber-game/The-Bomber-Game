drop database if exists TheBomber;
create database TheBomber;

use TheBomber;
drop table if exists account;

create table account (
username varchar(30) BINARY not null,
password varchar(100) not null
);
alter table account add primary key(username);

insert into account(username, password) values('Cyanic', MD5('123456'));
insert into account(username, password) values('Twofyw', MD5('123456'));
insert into account(username, password) values('YYYuna', MD5('123456'));
insert into account(username, password) values('novatez', MD5('123456'));