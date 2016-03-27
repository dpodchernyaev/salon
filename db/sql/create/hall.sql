CREATE SEQUENCE hall_id_seq;

DROP TABLE hall;

CREATE TABLE hall(
	"id" INTEGER PRIMARY KEY NOT NULL DEFAULT nextval('hall_id_seq'),
	"name" TEXT NOT NULL,
	"cnt" INTEGER NOT NULL DEFAULT 0
);