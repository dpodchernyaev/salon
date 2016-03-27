CREATE SEQUENCE service_id_seq;

DROP TABLE service;

CREATE TABLE service
(
	"id" integer PRIMARY KEY NOT NULL DEFAULT nextval('service_id_seq'),
	"name" TEXT NOT NULL,
	"price" REAL NOT NULL,
	"begin_date" DATE NOT NULL,
	"end_date" DATE NOT NULL,
	"limit_type" INTEGER NOT NULL,
	"value" INTEGER NOT NULL
);