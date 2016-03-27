CREATE SEQUENCE shedule_group_id_seq;

DROP TABLE shedule_group;

CREATE TABLE shedule_group(
	"id" INTEGER PRIMARY KEY NOT NULL DEFAULT nextval('shedule_group_id_seq'),
	"shedule_id" INTEGER NOT NULL,
	"cnt" INTEGER NOT NULL 
);

ALTER TABLE shedule_group
	ADD CONSTRAINT shedule_id_fk
		FOREIGN KEY (shedule_id) REFERENCES shedule (id)
			MATCH SIMPLE 
			ON UPDATE CASCADE 
			ON DELETE CASCADE;