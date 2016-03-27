CREATE SEQUENCE client_id_seq;

DROP TABLE client;

CREATE TABLE client
(
	"id" integer PRIMARY KEY NOT NULL DEFAULT nextval('client_id_seq'),
	"name" TEXT NOT NULL,
	"surname" TEXT NOT NULL,
	"patronymic" TEXT NOT NULL,
	"email" TEXT NOT NULL,
	"birthday" DATE NOT NULL,
	"phone" TEXT NOT NULL,
	"photo" TEXT NOT NULL,
	"info" TEXT NOT NULL
);