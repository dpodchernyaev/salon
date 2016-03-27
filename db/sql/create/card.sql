CREATE SEQUENCE coach_id_seq;

DROP TABLE coach;

CREATE TABLE coach(
	"id" INTEGER PRIMARY KEY NOT NULL DEFAULT nextval('coach_id_seq'),
	"name" TEXT NOT NULL,
);