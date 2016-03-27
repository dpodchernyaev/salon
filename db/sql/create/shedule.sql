CREATE SEQUENCE shedule_id_seq;

DROP TABLE shedule;

CREATE TABLE shedule(
	"id" INTEGER PRIMARY KEY NOT NULL DEFAULT nextval('shedule_id_seq'),
	"coach_id" INTEGER NOT NULL,
	"hall_id" INTEGER NOT NULL,
	"day" INTEGER NOT NULL,
	"begin_time" TIME NOT NULL,
	"end_time" TIME NOT NULL
);

ALTER TABLE shedule
	ADD CONSTRAINT hall_id_fk
		FOREIGN KEY (hall_id) REFERENCES hall (id)
			MATCH SIMPLE 
			ON UPDATE CASCADE 
			ON DELETE CASCADE;
			
ALTER TABLE shedule
	ADD CONSTRAINT coach_id_fk
		FOREIGN KEY (coach_id) REFERENCES coach (id)
			MATCH SIMPLE 
			ON UPDATE CASCADE 
			ON DELETE CASCADE;