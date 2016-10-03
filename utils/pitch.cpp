
double Pitch::getMidiNote()
{
	return 12.0 * log2(freq / 440.0) + 69.0;
}
int Pitch::getNearestMidiNote()
{
	return (int)round(getMidiNote());
}

int Pitch::getErrorInCents()
{
	return (int)std::abs(round(100.0 * (getMidiNote() - double(getNearestMidiNote()))));
}

String Pitch::getMidiNoteAsString(int midinote)
{
	static const char* notes[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
	int notenumber = midinote % 12;
	int octave = (midinote / 12) - 1; // Octave numbers are somewhat non-standard, one man's C3 is another man's C5.

	return String(notes[notenumber]) + String(octave);
}

int Pitch::getOctaveNumber()
{
	int midinote = getNearestMidiNote();
	return (midinote / 12) - 1;
}

bool Pitch::setFrequencyFromNoteName(const String& notename)
{
	static const char* notesSharp[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
	static const char* notesFlat[] = { "C", "DB", "D", "EB", "E", "F", "GB", "G", "AB", "A", "BB", "B" };

	const String s(notename.toUpperCase().retainCharacters("ABCDEFG#b-0123456789"));

	// Identify the octave
	const String octString(s.retainCharacters("-0123456789"));
	int octave(octString.getIntValue());

	if ((octave < -1) || (octave > 9) || (!octString.containsAnyOf("0123456789")))
	{
		octave = getOctaveNumber();
	}

	// Identify the note
	const String note(s.dropLastCharacters(octString.length()));
	int noteNumber(-1);

	for (int i = 0; i < 12; i++)
	{
		if (note == notesSharp[i] || note == notesFlat[i])
		{
			noteNumber = i;
			break;
		};
	}

	if (noteNumber == -1) return false;

	noteNumber += (octave + 1) * 12;

	freq = pow(2.0, (double(noteNumber) - 69.0) / 12.0) * 440.0;
	return true;
}
